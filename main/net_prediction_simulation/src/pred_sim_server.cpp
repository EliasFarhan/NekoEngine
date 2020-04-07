//
// Created by efarhan on 10/14/19.
//
#include "net_prediction_simulation/pred_sim_server.h"
#include "net_prediction_simulation/pred_sim_engine.h"
#include "imgui.h"
namespace neko::net
{

ServerSimSystem::ServerSimSystem(PredSimEngineExport& engineExport) : engineExport_(engineExport)
{
}

void ServerSimSystem::Init()
{
    auto& clientSystem_ = engineExport_.client;
    auto& entityManager = engineExport_.entityManager;
    auto& velocityManager = engineExport_.velocityManager;
    serverEntities_.reserve(actorNmb);
    serverActorsDataBuffer_.resize(actorNmb);

    auto& positionManager = engineExport_.position2dManager;
    auto& rotationManager = engineExport_.rotation2dManager;

    auto& shapeManager = engineExport_.shapeManager;
    int i = 0;
    for (auto& clientEntity : clientSystem_.entities_)
    {
        const auto entity = entityManager.CreateEntity();
        serverEntities_.push_back(entity);
        entitiesTranslateTable_[clientEntity] = entity;

        positionManager.AddComponent(entityManager, entity);
        const auto position = positionManager.GetComponent(clientEntity);
        positionManager.SetComponent(entity, position);

        rotationManager.AddComponent(entityManager, entity);
        rotationManager.SetComponent(entity, rotationManager.GetComponent(clientEntity));

        velocityManager.AddComponent(entityManager, entity);
        const auto velocity = velocityManager.GetComponent(clientEntity);
        velocityManager.SetComponent(entity, velocity);
        ActorData data;
        data.entity = clientEntity;
        data.tickIndex = 0;
        data.position = position;
        data.velocity = velocity;
        serverActorsDataBuffer_[i].fill(data);

        neko::sfml::ShapeDef serverShape;
        serverShape.fillColor = sf::Color::Blue;
        serverShape.fillColor.a = engineExport_.engine.globals_.actorAlpha;
        shapeManager.AddComponent(entityManager, entity);
        shapeManager.AddPolygon(entity, position, points, 3, serverShape);
        i++;
    }
    engineExport_.engine.drawUiDelegate_.RegisterCallback([this]([[maybe_unused]]float dt){
        auto& globals = engineExport_.engine.globals_;
        ImGui::Begin("Server Control");
        ImGui::LabelText("Bandwidth", "%f bytes/s", currentSecondBandwidth_);
        const char* serverPredictionName[] = {"None", "Interpolation", "Extrapolation", "Speed Interpolation", "Catmull Interpolation"};
        static int serverComboIndex = int(serverPredictionType_);
        if (ImGui::Combo("Prediction", &serverComboIndex, serverPredictionName, 4))
        {
            serverPredictionType_ = ServerPredictionType(serverComboIndex);
        }

        const char* clientDemoName[] = {"Linear", "Planet", "Boids"};
        static int clientComboIndex = int(engineExport_.client.clientMovementType_);
        if (ImGui::Combo("Movement", &clientComboIndex, clientDemoName, 3))
        {
			engineExport_.client.clientMovementType_ = ClientMovementType(clientComboIndex);
        }
        int tmpTickPeriod = globals.clientTickPeriod;
        if (ImGui::DragInt("Client Tick Rate", &tmpTickPeriod, 1, 1, 60, "Every %d frames"))
        {
            globals.clientTickPeriod = tmpTickPeriod;
        }
        ImGui::DragIntRange2("Packet Delay (frames)", &globals.packetDelayMin, &globals.packetDelayMax, 1, 0, 60);
        int tmpInterpolationPeriod = globals.serverDelayPeriod;
        if (ImGui::DragInt("Server Interpolation Delay", &tmpInterpolationPeriod, 1, 0, 60))
        {
            globals.serverDelayPeriod = tmpInterpolationPeriod;
        }
        int tmpShownActorNmb = globals.shownActorNmb;
        if(ImGui::DragInt("Shown Actor Nmb", &tmpShownActorNmb, 1, 0, int(actorNmb)))
        {
            globals.shownActorNmb = tmpShownActorNmb;
        }
        ImGui::DragFloat("Data Loss Probability", &globals.dataLossProb, 0.1f, 0.0f, 1.0f);
        ImGui::Checkbox("Hide Client", &globals.hideClient);
        ImGui::End();
    });
}

void ServerSimSystem::Update([[maybe_unused]] float dt)
{
    tick_++;
    if ((tick_ % engineExport_.engine.config.framerateLimit) == 0)
    {
        currentSecondBandwidth_ = float(dataSent_) / float(engineExport_.engine.config.framerateLimit);
        dataSent_ = 0;
    }

    auto& positionManager = engineExport_.position2dManager;
    auto& rotationManager = engineExport_.rotation2dManager;
    auto& velocityManager = engineExport_.velocityManager;
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
                const float newAngle = AngleBetween(velocity, Vec2f(0.0f, -1.0f));
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

                    const float newAngle = AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }
                else
                {
                    const auto position = lastActorData.position;
                    positionManager.SetComponent(entity, position);
                    const auto velocity = lastActorData.velocity;
                    velocityManager.SetComponent(entity, velocity);
                    const float newAngle = AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }

            }
            break;
        }
		case ServerPredictionType::SpeedInterpolation:
		{
			for (auto& entity : serverEntities_)
			{
				const auto index = Index(
					std::find(serverEntities_.cbegin(), serverEntities_.cend(), entity) - serverEntities_.cbegin());
				const auto lastActorData = serverActorsDataBuffer_[index].back();
				const auto previousActorData = serverActorsDataBuffer_[index][serverActorDataBufferSize - 2];
				const auto previousTick = previousActorData.tickIndex;
				const auto lastTick = lastActorData.tickIndex;
				if (lastTick > previousTick)
				{
					const float currentTime =
						(float(tick_) - float(previousTick)) / (float(lastTick) - float(previousTick));

					const auto velocity = neko::Vec2f::Lerp(previousActorData.velocity, lastActorData.velocity,
						currentTime);
					velocityManager.SetComponent(entity, velocity);
					
					const auto position = neko::Vec2f::Lerp(previousActorData.position, lastActorData.position,
						currentTime);
					positionManager.SetComponent(entity, position);



					const float newAngle = AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
					rotationManager.SetComponent(entity, newAngle);
				}
				else
				{
					const auto position = lastActorData.position;
					positionManager.SetComponent(entity, position);
					const auto velocity = lastActorData.velocity;
					velocityManager.SetComponent(entity, velocity);
					const float newAngle = AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
					rotationManager.SetComponent(entity, newAngle);
				}

			}
			break;
		}
        case ServerPredictionType::Interpolation:
        {
            neko::Index lastGlobalTick = tick_ > engineExport_.engine.globals_.serverDelayPeriod ? tick_ - engineExport_.engine.globals_.serverDelayPeriod : 0;

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

                    const float newAngle = AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }
                else
                {
                    const auto position = lastActorData.position;
                    positionManager.SetComponent(entity, position);
                    const auto velocity = lastActorData.velocity;
                    velocityManager.SetComponent(entity, velocity);
                    const float newAngle = AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }

            }
            break;
        }
        case ServerPredictionType::Catmull_Interpolation:
        {
            neko::Index lastGlobalTick = tick_ > engineExport_.engine.globals_.serverDelayPeriod ? tick_ - engineExport_.engine.globals_.serverDelayPeriod : 0;

            for (auto& entity : serverEntities_)
            {
                const auto index = neko::Index(
                        std::find(serverEntities_.cbegin(), serverEntities_.cend(), entity) - serverEntities_.cbegin());
                if(index >= engineExport_.engine.globals_.shownActorNmb)
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

                    const float newAngle = AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
                    rotationManager.SetComponent(entity, newAngle);
                }
                else
                {
                    const auto position = actorDataArray[2].position;
                    positionManager.SetComponent(entity, position);
                    const auto velocity = actorDataArray[2].velocity;
                    velocityManager.SetComponent(entity, velocity);
                    const float newAngle = AngleBetween(velocity, neko::Vec2f(0.0f, -1.0f));
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
    if (engineExport_.engine.globals_.distrNormal(eng_) < engineExport_.engine.globals_.dataLossProb)
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
}