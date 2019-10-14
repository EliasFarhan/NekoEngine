#include "net_prediction_simulation/pred_sim_engine.h"
#include "sfml_engine/physics.h"
#include <cmath>

#ifdef WIN32
#include <corecrt_math_defines.h>
#endif

#include <imgui.h>

namespace neko::net
{
PredSimEngine::PredSimEngine(neko::Configuration* config) : SfmlBasicEngine(config), server_(*this), client_(*this)
{

}

void PredSimEngine::Init()
{
    config.framerateLimit = 60u;
    config.vSync = false;
    SfmlBasicEngine::Init();
    client_.Init();
    server_.Init();
}

void PredSimEngine::Update(float dt)
{
    SfmlBasicEngine::Update(dt);
    client_.Update(dt);
    server_.Update(dt);

    shapeManager_.CopyTransformPosition(transformManager_.positionManager_, 0, actorNmb * 2);
    shapeManager_.CopyTransformRotation(transformManager_.rotationManager_, 0, actorNmb * 2);
    if(!globals_.hideClient)
    {
        shapeManager_.PushCommands(graphicsManager_, 0, globals_.shownActorNmb);
    }
    shapeManager_.PushCommands(graphicsManager_, actorNmb, globals_.shownActorNmb);

}

void PredSimEngine::Destroy()
{
    SfmlBasicEngine::Destroy();

    client_.Destroy();
    server_.Destroy();
}
}
