//
// Created by efarhan on 10/14/19.
//

#include "net_prediction_simulation/pred_sim_client.h"
#include "sfml_engine/physics.h"
#include "sfml_engine/shape.h"
#include "net_prediction_simulation/pred_sim_engine.h"
#include <cmath>

#ifdef WIN32
#include <corecrt_math_defines.h>
#endif
namespace neko::net
{

ClientSimSystem::ClientSimSystem(PredSimEngine& engine) : engine_(engine)
{

}

void ClientSimSystem::Init()
{

    auto& entityManager = engine_.entityManager_;
    auto& transformManager = engine_.transformManager_;
    auto& positionManager = transformManager.positionManager_;
    auto& rotationManager = transformManager.rotationManager_;
    auto& velocityManager = engine_.velocitiesManager_;
    auto& shapeManager = engine_.shapeManager_;

    const auto screenSize = engine_.config.gameWindowSize;

    std::uniform_real_distribution<float> distrX(0.0f, float(screenSize.first)); // define the range
    std::uniform_real_distribution<float> distrY(0.0f, float(screenSize.second)); // define the range
    std::uniform_real_distribution<float> distrAngle(0.0f, 2.0f * float(M_PI)); // define the range
    std::uniform_int_distribution<neko::Index> distrDirSwitch(60, 120); // define the range


    entities_.reserve(actorNmb);
    randomDirectionChangePeriods_.resize(actorNmb);
    for (size_t i = 0; i < actorNmb; i++)
    {
        const auto entity = entityManager.CreateEntity();
        entities_.push_back(entity);

        positionManager.AddComponent(entityManager, entity);
        const auto position = neko::pixel2unit(sf::Vector2f(distrX(eng_), distrY(eng_)));
        positionManager.SetComponent(entity, position);

        rotationManager.AddComponent(entityManager, entity);
        const float angle = distrAngle(eng_);
        rotationManager.SetComponent(entity, angle / float(M_PI) * 180.0f);

        velocityManager.AddComponent(entityManager, entity);
        velocityManager.SetComponent(entity, neko::Vec2f(sin(angle), -cos(angle)));

        neko::sfml::ShapeDef clientShape;
        clientShape.fillColor = sf::Color::Red;
        clientShape.fillColor.a = engine_.globals_.actorAlpha;

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
    auto& positionManager = transformManager.positionManager_;
    auto& rotationManager = transformManager.rotationManager_;
    auto& velocityManager = engine_.velocitiesManager_;
    const auto& globals = engine_.GetGlobals();
    const auto screenSize = engine_.config.gameWindowSize;
    const neko::Vec2f centerPos = neko::pixel2unit(
            sf::Vector2f(float(screenSize.first), float(screenSize.second))) / 2.0f;
    std::uniform_int_distribution<neko::Index> distrDelay(globals.packetDelayMin, globals.packetDelayMax); // define the range
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
                pos = positionManager.GetComponent(entity);
                vel = velocityManager.GetComponent(entity);
                const auto newPos = pos + vel * dt * linearSpeed;
                positionManager.SetComponent(entity, newPos);
                pos = newPos;
                if ((newPos.x < 0.0f and vel.x < 0.0f) or
                    (newPos.x > neko::pixel2unit(float(screenSize.first)) and vel.x > 0.0f))
                {
                    vel.x *= -1.0f;
                }
                if ((newPos.y < 0.0f and vel.y < 0.0f) or
                    (newPos.y > neko::pixel2unit(float(screenSize.second)) and vel.y > 0.0f))
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
                pos = positionManager.GetComponent(entity);
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

                pos = positionManager.GetComponent(entity);
                for (const auto otherEntity : entities_)
                {
                    if (otherEntity == entity)
                    {
                        continue;
                    }
                    const auto deltaPos = positionManager.GetComponent(otherEntity) - pos;
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
                        totalVel += velocityManager.GetComponent(entity).Normalized();
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

                vel = velocityManager.GetComponent(entity);
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


        if (tick_ % globals.clientTickPeriod == 0)
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
}