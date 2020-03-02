#include "logger/logger_engine.h"

namespace neko
{
LoggerEngine::LoggerEngine(Configuration* config) : SdlEngine(config)
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

void LoggerEngine::OnEvent(const SDL_Event& event)
{
    sampleBrowser_.OnEvent(event);
}
}
