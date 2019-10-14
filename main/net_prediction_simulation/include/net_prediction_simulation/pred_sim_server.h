#pragma once
#include <array>
#include <random>
#include <engine/system.h>
#include <unordered_map>
#include "actor_data.h"

namespace neko::net
{
class PredSimEngine;
class ServerSimSystem : public neko::System
{
public:
    explicit ServerSimSystem(PredSimEngine& engine);
    void Init() override;
    void Update(float dt) override;
    void Destroy() override;
    void PushClientData(const ActorData& data);
private:
    PredSimEngine& engine_;
    std::vector<std::array<ActorData, serverActorDataBufferSize>> serverActorsDataBuffer_;
    std::vector<neko::Entity> serverEntities_;
    std::unordered_map<neko::Entity, neko::Entity> entitiesTranslateTable_;
    neko::Index tick_ = 0;
    size_t dataSent_ = 0;
    float currentSecondBandwidth_ = 0.0f;
    std::random_device rd{};
    std::mt19937 eng_{rd()};
    ServerPredictionType serverPredictionType_ = ServerPredictionType::Interpolation;
};
}