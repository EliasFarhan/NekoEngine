#pragma once
#include <vector>
#include <random>
#include <engine/system.h>
#include <engine/globals.h>

#include "net_prediction_simulation/actor_data.h"

namespace neko::net
{
class PredSimEngine;
class ClientSimSystem : public  neko::System
{
public:
    explicit ClientSimSystem(PredSimEngine& engine);
    void Init() override;
    void Update(float dt) override;
    void Destroy() override;
private:
    friend class ServerSimSystem;
    std::vector<neko::Index> randomDirectionChangePeriods_;
    std::vector<std::pair<ActorData, neko::Index>> dataDelayQueue_;
    std::vector<neko::Entity> entities_;
    PredSimEngine& engine_;
    neko::Index tick_ = 0;
    ClientMovementType clientMovementType_ = ClientMovementType::Boids;
    std::random_device rd{};
    std::mt19937 eng_{rd()};
};
}