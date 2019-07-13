//
// Created by efarhan on 13.07.19.
//

#include <MultiThreadEngine/engine.h>

namespace multi
{

MainEngine* MainEngine::engine_ = nullptr;


void MainEngine::EngineLoop()
{

    isRunning = true;
    renderThread_ = std::thread(&MultiThreadGraphicsManager::RenderLoop, graphicsManager_.get());
    renderThread_.detach();
    while (isRunning)
    {
        rmt_ScopedCPUSample(EngineLoop, 0);
        clockDeltatime = engineClock_.restart();
        if (frameIndex > 0)
        {
            rmt_ScopedCPUSample(WaitForGraphics, 0);
            std::unique_lock<std::mutex> lock(renderStartMutex);
            if (graphicsManager_->DidRenderingStart())
            {
                condSyncRender.notify_all();
            }
            else
            {
                //Wait until the graphics manager is ready to draw the next frame
                continue;
            }
        }
        keyboardManager_.ClearKeys();
        mouseManager_.ClearFrameData();
        sf::Event event{};
        while (renderWindow->pollEvent(event))
        {
            OnEvent(event);
            ImGui::SFML::ProcessEvent(event);
        }
        Update(clockDeltatime.asSeconds());

        if (frameIndex > 0)
        {
            //wait for the end of the rendering
            std::unique_lock<std::mutex> lock(graphicsManager_->renderingMutex);
        }

        ++frameIndex;
    }

    Destroy();
}

MainEngine::MainEngine(neko::Configuration* config) : BasicEngine(config)
{

}

MainEngine::~MainEngine()
{

    logDebug("Destroy Main Engine");


}

void MainEngine::Init()
{
    //workingThreadPool.resize(std::max(1u,std::thread::hardware_concurrency() - 3));//removing main and render and audio thread
#ifdef __linux__
    XInitThreads();
#endif
    BasicEngine::Init();
    renderWindow->setActive(false);
    instance_ = this;

    graphicsManager_ = std::make_unique<MultiThreadGraphicsManager>();


}

void MainEngine::Update(float dt)
{

}

void MainEngine::Destroy()
{
    {
        std::unique_lock<std::mutex> lock(graphicsManager_->renderingMutex);
    }
    renderWindow->setActive(true);

    BasicEngine::Destroy();
    instance_ = nullptr;
    graphicsManager_ = nullptr;
}

GraphicsManager* MainEngine::GetGraphicsManager() const
{
    return graphicsManager_.get();
}

MainEngine* MainEngine::GetInstance()
{
    return instance_;
}

}