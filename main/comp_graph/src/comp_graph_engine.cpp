#include "comp_graph/comp_graph_engine.h"

namespace neko
{
CompGraphEngine::CompGraphEngine(Configuration* config) : SdlEngine(config)
{
    initAction_.RegisterCallback(
            [this](void)
            {
                helloTriangleCommand_.Init();
            });
    updateAction_.RegisterCallback(
            [this](seconds dt)
            {
                helloTriangleCommand_.Update(dt);
            });
    destroyAction_.RegisterCallback(
            [this](void)
            {
                helloTriangleCommand_.Destroy();
            });
    drawAction_.RegisterCallback(
            [this]()
            {
                helloTriangleCommand_.Render();
            });
}

void CompGraphEngine::OnEvent(const SDL_Event& event)
{

}
}
