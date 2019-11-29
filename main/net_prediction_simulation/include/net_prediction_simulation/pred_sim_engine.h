#pragma once


#include <engine/engine.h>
#include <engine/transform.h>
#include "sfml_engine/shape.h"
#include <random>
#include <sfml_engine/transform.h>
#include <sfml_engine/engine.h>

#include "net_prediction_simulation/pred_sim_server.h"
#include "net_prediction_simulation/pred_sim_client.h"

namespace neko::net
{

class VelocityManager : public neko::ComponentManager<neko::Vec2f, velocityMask>
{
	using ComponentManager::ComponentManager;
};

struct Globals
{
    size_t shownActorNmb = 100;
    sf::Uint8 actorAlpha = 150;
    neko::Index clientTickPeriod = 2;
    int packetDelayMin = 2;
    int packetDelayMax = 20;
    std::uniform_real_distribution<float> distrNormal{0.0f, 1.0f};
    float dataLossProb = 0.0f;
    neko::Index serverDelayPeriod = 9;
    bool hideClient = false;
};


class PredSimEngine : public neko::sfml::SfmlBasicEngine
{
public:
    explicit PredSimEngine(neko::Configuration* config = nullptr);

    void Init() override;


    void Destroy() override;
    const Globals& GetGlobals(){ return globals_; }
private:
    friend class ServerSimSystem;

    friend class ClientSimSystem;
	PredSimEngineExport engineExport_;
    ServerSimSystem server_;
    ClientSimSystem client_;
    Globals globals_;
	
    EntityManager entityManager_;
    sfml::Transform2dManager transformManager_;
	Position2dManager positionManager_;
	Scale2dManager scaleManager_;
	Rotation2dManager rotationManager_;
	sfml::ConvexShapeManager shapeManager_;
    VelocityManager velocitiesManager_;
};

}
