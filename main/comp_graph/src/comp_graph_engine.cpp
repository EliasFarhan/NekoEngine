#include "comp_graph/comp_graph_engine.h"

namespace neko
{
CompGraphEngine::CompGraphEngine(Configuration* config) : SdlEngine(config)
{
    initAction_.RegisterCallback(
            [this](void) {
                sampleBrowser_.Init();
            });
    updateAction_.RegisterCallback(
            [this](seconds dt) {
                sampleBrowser_.Update(dt);
            });
    destroyAction_.RegisterCallback(
            [this](void) {
                sampleBrowser_.Destroy();
            });
    drawUiAction_.RegisterCallback(
            [this]() {
                sampleBrowser_.DrawGui();
            });
}

void CompGraphEngine::OnEvent(const SDL_Event& event)
{
    sampleBrowser_.OnEvent(event);
}

void CompGraphEngine::Destroy()
{
    sampleBrowser_.Destroy();
    SdlEngine::Destroy();
}
}
