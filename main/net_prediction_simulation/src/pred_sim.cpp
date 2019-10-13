#include "net_prediction_simulation/pred_sim.h"
#include <cmath>

#ifdef WIN32
#include <corecrt_math_defines.h>
#endif

#include <imgui.h>

namespace net
{
const size_t actorNmb = 100;
size_t shownActorNmb = 100;
const float linearSpeed = 2.0f;
const neko::Vec2f points[] = {
        neko::Vec2f(0.0f, -0.2f),
        neko::Vec2f(0.2f, 0.2f),
        neko::Vec2f(-0.2f, 0.2f)
};
sf::Uint8 actorAlpha = 150;
neko::Index clientTickPeriod = 2;
neko::Index serverDelayPeriod = 9;
int packetDelayMin = 2;
int packetDelayMax = 20;
std::uniform_real_distribution<float> distrNormal(0.0f, 1.0f);
float dataLossProb = 0.0f;
const float gravityConst = 1.0f;
const float centerMass = 10.0f;
const float planetMass = 1.0f;
bool hideClient = false;

ServerSimSystem::ServerSimSystem(PredSimEngine& engine) : engine_(engine)
{
}

void ServerSimSystem::Init()
{
    auto& clientSystem_ = engine_.client_;
    auto& entityManager = engine_.entityManager_;
    auto& transformManager = engine_.transformManager_;
    auto& positionManager = transformManager.GetPositionManager();
    auto& rotationManager = transformManager.GetRotationManager();
    auto& velocityManager = engine_.velocitiesManager_;
    serverEntities_.reserve(actorNmb);
    serverActorsDataBuffer_.resize(actorNmb);

    auto& shapeManager = engine_.shapeManager_;
    int i = 0;
    for (auto& clientEntity : clientSystem_.entities_)
    {
        const auto entity = entityManager.CreateEntity();
        serverEntities_.push_back(entity);
        entitiesTranslateTable_[clientEntity] = entity;

        positionManager.AddComponent(entityManager, entity);
        const auto position = positionManager.GetConstComponent(clientEntity);
        positionManager.SetComponent(entity, position);

        rotationManager.AddComponent(entityManager, entity);
        rotationManager.SetComponent(entity, rotationManager.GetConstComponent(clientEntity));

        velocityManager.AddComponent(entityManager, entity);
        const auto velocity = velocityManager.GetConstComponent(clientEntity);
        velocityManager.SetComponent(entity, velocity);
        ActorData data;
        data.entity = clientEntity;
        data.tickIndex = 0;
        data.position = position;
        data.velocity = velocity;
        serverActorsDataBuffer_[i].fill(data);

        neko::ShapeDef serverShape;
        serverShape.fillColor = sf::Color::Blue;
        serverShape.fillColor.a = actorAlpha;
        shapeManager.AddComponent(entityManager, entity);
        shapeManager.AddPolygon(entity, position, points, 3, serverShape);
        i++;
    }
}

void ServerSimSystem::Update([[maybe_unused]] float dt)
{
    tick_++;
    if ((tick_ % engine_.config.framerateLimit) == 0)
    {
        currentSecondBandwidth_ = float(dataSent_) / float(engine_.config.framerateLimit);
        dataSent_ = 0;
    }
    ImGui::Begin("Server Control");
    ImGui::LabelText("Bandwidth", "%f bytes/s", currentSecondBandwidth_);
    const char* serverPredictionName[] = {"None", "Interpolation", "Extrapolation", "Catmull Interpolation"};
    static int serverComboIndex = int(serverPredictionType_);
    if (ImGui::Combo("Prediction", &serverComboIndex, serverPredictionName, 4))
    {
        serverPredictionType_ = ServerPredictionType(serverComboIndex);
    }

    const char* clientDemoName[] = {"Linear", "Planet", "Boids"};
    static int clientComboIndex = int(engine_.client_.clientMovementType_);
    if (ImGui::Combo("Movement", &clientComboIndex, clientDemoName, 3))
    {
        engine_.client_.clientMovementType_ = ClientMovementType(clientComboIndex);
    }
    int tmpTickPeriod = clientTickPeriod;
    if (ImGui::DragInt("Client Tick Rate", &tmpTickPeriod, 1, 1, 60, "Every %d frames"))
    {
        clientTickPeriod = tmpTickPeriod;
    }
    ImGui::DragIntRange2("Packet Delay (frames)", &packetDelayMin, &packetDelayMax, 1, 0, 60);
    int tmpInterpolationPeriod = serverDelayPeriod;
    if (ImGui::DragInt("Server Interpolation Delay", &tmpInterpolationPeriod, 1, 0, 60))
    {
        serverDelayPeriod = tmpInterpolationPeriod;
    }
    int tmpShownActorNmb = shownActorNmb;
    if(ImGui::DragInt("Shown Actor Nmb", &tmpShownActorNmb, 1, 0, int(actorNmb)))
    {
        shownActorNmb = tmpShownActorNmb;
    }
    ImGui::DragFloat("Data Loss Probability", &dataLossProb, 0.1f, 0.0f, 1.0f);
    ImGui::Checkbox("Hide Client", &hideClient);
    ImGui::End();
    auto& transformManager = engine_.transformManager_;
    auto& positionManager = transformManager.GetPositionManager();
    auto& rotationManager = transformManager.GetRotationManager();
    auto& velocityManager = engine_.velocitiesManager_;
    switch (serverPredictionType_)
    {
        case ServerPredictionType::None:
        {
            for (auto& entity : serverEntities_)
            {
                const auto index = neko::Index(
                        std::find(serverEntities_.cbegin(), serverEntities_.cend(), entity) - serverEntities_.cbegin());
                const auto actorData = serverActorsDataBuffer_[index].back();
                const auto position = actorData.position;
                positionManager.SetComponent(entity, position);
                const auto velocity = actorData.velocity;
                velocityManager.SetComponent(entity, velocity);
                const float newAngle = neko::Vec2f::AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                rotationManager.SetComponent(entity, newAngle);
            }
            break;
        }
        case ServerPredictionType::Extrapolation:
        {
            for (auto& entity : serverEntities_)
            {
                const auto index = neko::Index(
                        std::find(serverEntities_.cbegin(), serverEntities_.cend(), entity) - serverEntities_.cbegin());
                const auto lastActorData = serverActorsDataBuffer_[index].back();
                const auto previousActorData = serverActorsDataBuffer_[index][serverActorDataBufferSize-2];
                const auto previousTick = previousActorData.tickIndex;
                const auto lastTick = lastActorData.tickIndex;
                if (lastTick > previousTick)
                {
                    const float currentTime =
                            (float(tick_) - float(previousTick)) / (float(lastTick) - float(previousTick));

                    const auto position = neko::Vec2f::Lerp(previousActorData.position, lastActorData.position,
                                                            currentTime);
                    positionManager.SetComponent(entity, position);

                    const auto velocity = neko::Vec2f::Lerp(previousActorData.velocity, lastActorData.velocity,
                                                            currentTime);
                    velocityManager.SetComponent(entity, velocity);

                    const float newAngle = neko::Vec2f::AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }
                else
                {
                    const auto position = lastActorData.position;
                    positionManager.SetComponent(entity, position);
                    const auto velocity = lastActorData.velocity;
                    velocityManager.SetComponent(entity, velocity);
                    const float newAngle = neko::Vec2f::AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }

            }
            break;
        }
        case ServerPredictionType::Interpolation:
        {
            neko::Index lastGlobalTick = tick_ > serverDelayPeriod ? tick_ - serverDelayPeriod : 0;

            for (auto& entity : serverEntities_)
            {
                const auto index = neko::Index(
                        std::find(serverEntities_.cbegin(), serverEntities_.cend(), entity) - serverEntities_.cbegin());
                const auto lastActorData = serverActorsDataBuffer_[index].back();
                const auto previousActorData = serverActorsDataBuffer_[index][serverActorDataBufferSize-2];
                const auto previousTick = previousActorData.tickIndex;
                const auto lastTick = lastActorData.tickIndex;
                if (lastTick > previousTick)
                {
                    const float currentTime =
                            (float(lastGlobalTick) - float(previousTick)) / (float(lastTick) - float(previousTick));

                    const auto position = neko::Vec2f::Lerp(previousActorData.position, lastActorData.position,
                                                            currentTime);
                    positionManager.SetComponent(entity, position);

                    const auto velocity = neko::Vec2f::Lerp(previousActorData.velocity, lastActorData.velocity,
                                                            currentTime);
                    velocityManager.SetComponent(entity, velocity);

                    const float newAngle = neko::Vec2f::AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }
                else
                {
                    const auto position = lastActorData.position;
                    positionManager.SetComponent(entity, position);
                    const auto velocity = lastActorData.velocity;
                    velocityManager.SetComponent(entity, velocity);
                    const float newAngle = neko::Vec2f::AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }

            }
            break;
        }
        case ServerPredictionType::Catmull_Interpolation:
        {
            neko::Index lastGlobalTick = tick_ > serverDelayPeriod ? tick_ - serverDelayPeriod : 0;

            for (auto& entity : serverEntities_)
            {
                const auto index = neko::Index(
                        std::find(serverEntities_.cbegin(), serverEntities_.cend(), entity) - serverEntities_.cbegin());
                if(index >= shownActorNmb)
                    continue;
                const auto& actorDataArray = serverActorsDataBuffer_[index];
                const auto t0 = actorDataArray[0].tickIndex;
                const auto t1 = actorDataArray[1].tickIndex;
                const auto t2 = actorDataArray[2].tickIndex;
                const auto t3 = actorDataArray[3].tickIndex;
                const auto t = lastGlobalTick;
                if (t3 > t2 and t2 > t1 and t1 > t0)
                {
                    const auto framePeriod = float(t2-t1);
                    const float currentTime = (float(lastGlobalTick) - float(t1)) / framePeriod;

                    const auto& p0 = actorDataArray[0].position;
                    const auto& p1 = actorDataArray[1].position;
                    const auto& p2 = actorDataArray[2].position;
                    const auto& p3 = actorDataArray[3].position;
                    const auto A1 = p0*float(t1-t)/float(t1-t0) + p1*float(t-t0)/float(t1-t0);
                    const auto A2 = p1*float(t2-t)/float(t2-t1) + p2*float(t-t1)/float(t2-t1);
                    const auto A3 = p2*float(t3-t)/float(t3-t2) + p3*float(t-t2)/float(t3-t2);

                    const auto B1 = A1*float(t2-t)/float(t2-t0) + A2*float(t-t0)/float(t2-t0);
                    const auto B2 = A2*float(t3-t)/float(t3-t1) + A3*float(t-t1)/float(t3-t1);

                    const auto position = B1*float(t2-t)/float(t2-t1) + B2*float(t-t1)/float(t2-t1);
                    positionManager.SetComponent(entity, position);

                    const auto velocity = neko::Vec2f::Lerp(actorDataArray[2].velocity, actorDataArray[1].velocity, currentTime);
                    velocityManager.SetComponent(entity, velocity);

                    const float newAngle = neko::Vec2f::AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }
                else
                {
                    const auto position = actorDataArray[2].position;
                    positionManager.SetComponent(entity, position);
                    const auto velocity = actorDataArray[2].velocity;
                    velocityManager.SetComponent(entity, velocity);
                    const float newAngle = neko::Vec2f::AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }
            }
            break;
        }
        default:;
    }
}

void ServerSimSystem::Destroy()
{
}

void ServerSimSystem::PushClientData(const ActorData& data)
{
    //Dropping random packet
    if (distrNormal(eng_) < dataLossProb)
    {
        return;
    }
    const auto clientEntity = data.entity;
    const auto entity = entitiesTranslateTable_[clientEntity];
    const auto index = neko::Index(
            std::find(serverEntities_.cbegin(), serverEntities_.cend(), entity) - serverEntities_.cbegin());
    auto& dataArray = serverActorsDataBuffer_[index];
    for (int i = serverActorDataBufferSize - 1; i >= 0; i--)
    {
        if (dataArray[i].tickIndex < data.tickIndex)
        {
            for (int j = 0; j < i; j++)
            {
                dataArray[j] = dataArray[1 + j];
            }
            dataArray[i] = data;
            break;
        }
    }
    dataSent_ += sizeof(dataArray);
}

ClientSimSystem::ClientSimSystem(PredSimEngine& engine) : engine_(engine)
{

}

void ClientSimSystem::Init()
{

    auto& entityManager = engine_.entityManager_;
    auto& transformManager = engine_.transformManager_;
    auto& positionManager = transformManager.GetPositionManager();
    auto& rotationManager = transformManager.GetRotationManager();
    auto& velocityManager = engine_.velocitiesManager_;
    auto& shapeManager = engine_.shapeManager_;

    const auto screenSize = engine_.config.gameWindowSize;

    std::uniform_real_distribution<float> distrX(0.0f, float(screenSize.x)); // define the range
    std::uniform_real_distribution<float> distrY(0.0f, float(screenSize.y)); // define the range
    std::uniform_real_distribution<float> distrAngle(0.0f, 2.0f * float(M_PI)); // define the range
    std::uniform_int_distribution<neko::Index> distrDirSwitch(60, 120); // define the range


    entities_.reserve(actorNmb);
    randomDirectionChangePeriods_.resize(actorNmb);
    for (size_t i = 0; i < actorNmb; i++)
    {
        const auto entity = entityManager.CreateEntity();
        entities_.push_back(entity);

        positionManager.AddComponent(entityManager, entity);
        const auto position = neko::pixel2unit(neko::Vec2f(distrX(eng_), distrY(eng_)));
        positionManager.SetComponent(entity, position);

        rotationManager.AddComponent(entityManager, entity);
        const float angle = distrAngle(eng_);
        rotationManager.SetComponent(entity, angle / float(M_PI) * 180.0f);

        velocityManager.AddComponent(entityManager, entity);
        velocityManager.SetComponent(entity, neko::Vec2f(sin(angle), -cos(angle)));

        neko::ShapeDef clientShape;
        clientShape.fillColor = sf::Color::Red;
        clientShape.fillColor.a = actorAlpha;

        shapeManager.AddComponent(entityManager, entity);
        shapeManager.AddPolygon(entity, position, points, 3, clientShape);
        randomDirectionChangePeriods_[entity] = distrDirSwitch(eng_);
    }
}

void ClientSimSystem::Update(float dt)
{
    tick_++;
    auto delayDataIt = dataDelayQueue_.begin();
    while (delayDataIt != dataDelayQueue_.end())
    {
        delayDataIt->second--;
        if (delayDataIt->second == 0)
        {
            engine_.server_.PushClientData(delayDataIt->first);
            delayDataIt = dataDelayQueue_.erase(delayDataIt);
        }
        else
        {
            ++delayDataIt;
        }
    }

    auto& transformManager = engine_.transformManager_;
    auto& positionManager = transformManager.GetPositionManager();
    auto& rotationManager = transformManager.GetRotationManager();
    auto& velocityManager = engine_.velocitiesManager_;
    const auto screenSize = engine_.config.gameWindowSize;
    const neko::Vec2f centerPos = neko::pixel2unit(
            neko::Vec2f(float(screenSize.x), float(screenSize.y))) / 2.0f;
    std::uniform_int_distribution<neko::Index> distrDelay(packetDelayMin, packetDelayMax); // define the range
    std::uniform_int_distribution<neko::Index> distrDirSwitch(60, 120); // define the range
    std::uniform_real_distribution<float> distrAngle(0.0f, 2.0f * float(M_PI)); // define the range


// seed the generator
    for (auto& entity : entities_)
    {
        neko::Vec2f pos;
        neko::Vec2f vel;
        switch (clientMovementType_)
        {
            case ClientMovementType::Linear:
            {
                randomDirectionChangePeriods_[entity]--;
                if (randomDirectionChangePeriods_[entity] == 0)
                {
                    const float angle = distrAngle(eng_);
                    rotationManager.SetComponent(entity, angle / float(M_PI) * 180.0f);
                    velocityManager.SetComponent(entity, neko::Vec2f(sin(angle), -cos(angle)));
                    randomDirectionChangePeriods_[entity] = distrDirSwitch(eng_);
                }
                pos = positionManager.GetConstComponent(entity);
                vel = velocityManager.GetConstComponent(entity);
                const auto newPos = pos + vel * dt * linearSpeed;
                positionManager.SetComponent(entity, newPos);
                pos = newPos;
                if ((newPos.x < 0.0f and vel.x < 0.0f) or
                    (newPos.x > neko::pixel2unit(float(screenSize.x)) and vel.x > 0.0f))
                {
                    vel.x *= -1.0f;
                }
                if ((newPos.y < 0.0f and vel.y < 0.0f) or
                    (newPos.y > neko::pixel2unit(float(screenSize.y)) and vel.y > 0.0f))
                {
                    vel.y *= -1.0f;
                }

                const float newAngle = neko::Vec2f::AngleBetween(vel, neko::Vec2f(0.0f, -1.0f));
                rotationManager.SetComponent(entity, newAngle);
                velocityManager.SetComponent(entity, vel);
                break;
            }
            case ClientMovementType::Planet:
            {
                pos = positionManager.GetConstComponent(entity);
                const auto deltaPos = pos - centerPos;
                const auto r = deltaPos.GetMagnitude();
                const auto forceScalar = gravityConst * centerMass * planetMass / (r * r);
                auto velDir = neko::Vec2f(-deltaPos.y, deltaPos.x);
                velDir = velDir.Normalized();

                const auto planetSpeed = sqrtf(forceScalar / planetMass * r);
                vel = velDir * planetSpeed;

                const auto newPos = pos + vel * dt * linearSpeed;
                pos = newPos;
                positionManager.SetComponent(entity, newPos);

                const float newAngle = neko::Vec2f::AngleBetween(vel.Normalized(), neko::Vec2f(0.0f, -1.0f));
                rotationManager.SetComponent(entity, newAngle);
                velocityManager.SetComponent(entity, vel);
                break;
            }
            //Separation, Alignement and Cohesion
            case ClientMovementType::Boids:
            {
                const float localSeparationRadius = 1.0f;
                const float localCohesionRadius = 1.0f;
                const float localAlignementRadius = 1.0f;
                const float centerPullFactor = 2.0f;
                const float separationFactor = 2.0f / float(actorNmb);
                const float cohesionFactor = 1.0f / float(actorNmb);
				const float alignementFactor = 2.0f / float(actorNmb);
                neko::Vec2f totalDeltaPos = neko::Vec2f();
                neko::Vec2f totalVel = neko::Vec2f();
                neko::Vec2f totalSeparate = neko::Vec2f();
                size_t cohesionActorNmb = 0;
                size_t separationActorNmb = 0;
                size_t alignementActorNmb = 0;

                neko::Vec2f acceleration = neko::Vec2f();

                pos = positionManager.GetConstComponent(entity);
                for (const auto otherEntity : entities_)
                {
                    if (otherEntity == entity)
                    {
                        continue;
                    }
                    const auto deltaPos = positionManager.GetConstComponent(otherEntity)-pos;
                    const auto distance = deltaPos.GetMagnitude();
                    const auto dir = deltaPos.Normalized();
                    if(distance < localSeparationRadius)
                    {
                        totalSeparate -= dir * (localSeparationRadius - distance) / localSeparationRadius;
                        separationActorNmb++;
                    }
                    if(distance < localCohesionRadius)
                    {
                        totalDeltaPos += deltaPos;
                        cohesionActorNmb++;
                    }
                    if(distance < localAlignementRadius)
                    {
                        totalVel += velocityManager.GetConstComponent(entity).Normalized();
                        alignementActorNmb++;
                    }
                }
                if(separationActorNmb > 0)
                {
                    totalSeparate /= float(separationActorNmb);
                    acceleration += totalSeparate * separationFactor;
                }
                //Cohesion
                if(cohesionActorNmb > 0)
                {
                    totalDeltaPos /= float(cohesionActorNmb);
                    acceleration += totalDeltaPos * cohesionFactor;
                }

                vel = velocityManager.GetConstComponent(entity);
                //Alignement
                if(alignementActorNmb > 0)
                {
                    totalVel /= float(alignementActorNmb);
                    acceleration += (totalVel-vel)*alignementFactor;
                }
                //Pull by center
                if((centerPos-pos).GetMagnitude() > localCohesionRadius)
                {
                    acceleration += (centerPos - pos).Normalized() * centerPullFactor;
                }
                vel = (acceleration*dt+vel).Normalized();
                velocityManager.SetComponent(entity, vel);
                const auto newPos = pos + vel * dt * linearSpeed;
                pos = newPos;
                positionManager.SetComponent(entity, newPos);
                const float newAngle = neko::Vec2f::AngleBetween(vel, neko::Vec2f(0.0f, -1.0f));
                rotationManager.SetComponent(entity, newAngle);
                break;
            }
            default:;
        }


        if (tick_ % clientTickPeriod == 0)
        {
            ActorData data;
            data.tickIndex = tick_;
            data.entity = entity;
            data.position = pos;
            data.velocity = vel;

            dataDelayQueue_.emplace_back(data, distrDelay(eng_));
        }
    }
}

void ClientSimSystem::Destroy()
{
}

PredSimEngine::PredSimEngine() : server_(*this), client_(*this)
{

}

void PredSimEngine::Init()
{
    config.framerateLimit = 60u;
    config.vSync = false;
    BasicEngine::Init();

    client_.Init();
    server_.Init();
}

void PredSimEngine::Update(float dt)
{
    BasicEngine::Update(dt);
    client_.Update(dt);
    server_.Update(dt);

    shapeManager_.CopyTransformPosition(transformManager_.GetPositionManager(), 0, actorNmb * 2);
    shapeManager_.CopyTransformRotation(transformManager_.GetRotationManager(), 0, actorNmb * 2);
    if(!hideClient)
    {
        shapeManager_.PushCommands(&graphicsManager_, 0, shownActorNmb);
    }
    shapeManager_.PushCommands(&graphicsManager_, actorNmb, shownActorNmb);

    graphicsManager_.Render(*renderWindow);
}

void PredSimEngine::Destroy()
{
    BasicEngine::Destroy();

    client_.Destroy();
    server_.Destroy();
}
}
