#pragma once
#include <vector>
#include <random>
#include <engine/system.h>
#include <engine/globals.h>

#include "net_prediction_simulation/actor_data.h"
#include "net_prediction_simulation/pred_engine_export.h"

namespace neko::net
{
struct PredSimEngineExport;
class ClientSimSystem : public  neko::System
{
public:
    explicit ClientSimSystem(PredSimEngineExport& engineExport);
    void Init() override;
    void Update(float dt) override;
    void Destroy() override;
private:
    friend class ServerSimSystem;
    std::vector<neko::Index> randomDirectionChangePeriods_;
    std::vector<std::pair<ActorData, neko::Index>> dataDelayQueue_;
    std::vector<neko::Entity> entities_;
    neko::Index tick_ = 0;
    ClientMovementType clientMovementType_ = ClientMovementType::Linear;
	PredSimEngineExport& engineExport_;
    std::random_device rd{};
    std::mt19937 eng_{rd()};
};
}