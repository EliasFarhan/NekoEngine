#include <graphics/graphics.h>
#include <engine/engine.h>

namespace neko
{


void GraphicsManager::RenderLoop(std::condition_variable &condSyncRender,
                                 std::mutex &renderMutex)
{
    auto* engine = Engine::GetInstance();
    auto* renderWindow = Engine::GetInstance()->renderWindow;
    renderWindow->setActive(true);
    while (engine->isRunning)
    {

        std::unique_lock<std::mutex> lock(renderMutex);

        using namespace std::chrono_literals;
        condSyncRender.wait_for(lock, 1ms);
        renderWindow = Engine::GetInstance()->renderWindow;
        if (renderWindow != nullptr && renderWindow->isOpen())
        {
            renderWindow->clear(sf::Color::Black);
            //TODO manage command buffers
            renderWindow->display();
        }
    }
}
}