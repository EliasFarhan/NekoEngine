#include <MultiThreadEngine/graphics.h>

namespace multi
{


MultiThreadGraphicsManager::MultiThreadGraphicsManager()
{
    commandBuffers_[0].fill(nullptr);
    commandBuffers_[1].fill(nullptr);
    editor = std::make_unique<Editor>();
}

void MultiThreadGraphicsManager::Draw(sf::Drawable& drawable)
{
    if (nextRenderLength_ >= MAX_COMMAND_NMB)
    {
        logDebug("[Error] Too many draw calls compare to MAX_COMMAND_NMB");
        return;
    }
    const Index index = MainEngine::GetInstance()->frameIndex % 2;
    /*{
        std::ostringstream oss;
        oss << "Graphics Command On Engine Frame: " << MainEngine::GetInstance()->frameIndex <<" and Graphics frame: "<<frameIndex<<" with render length: "<< nextRenderLength_;
        logDebug(oss.str());
    }*/
    SfmlCommand command;
    command.drawable = &drawable;
    commands_[index][nextRenderLength_] = command;
    commandBuffers_[index][nextRenderLength_] = &commands_[index][nextRenderLength_];

    nextRenderLength_++;
}


void MultiThreadGraphicsManager::Draw(sf::VertexArray* vertexArray, sf::Texture* texture)
{
    const int index = MainEngine::GetInstance()->frameIndex % 2;
    TilemapCommand tilemapCommand;
    tilemapCommand.vertexArray = vertexArray;
    tilemapCommand.texture = texture;

    tileCommands_[index][nextRenderLength_] = tilemapCommand;
    commandBuffers_[index][nextRenderLength_] = &tileCommands_[index][nextRenderLength_];

    nextRenderLength_++;
}

void MultiThreadGraphicsManager::SetView(sf::View view)
{
    const int index = MainEngine::GetInstance()->frameIndex % 2;
    views_[index] = view;
}

void MultiThreadGraphicsManager::RenderLoop()
{
    auto* engine = MainEngine::GetInstance();

    renderWindow_ = engine->renderWindow.get();
    renderWindow_->setActive(true);
    views_[0] = renderWindow_->getView();
    views_[1] = views_[0];
    editor->graphicsManager_ = this;
    editor->renderWindow_ = renderWindow_;
    renderWindow_->setActive(false);

    do
    {
        rmt_ScopedCPUSample(RenderLoop, 0);

        {
            /*{
                std::ostringstream oss;
                oss << "Graphics Frame Start: " << MainEngine::GetInstance()->frameIndex << " and Graphics frame: " << frameIndex;
                logDebug(oss.str());
            }*/
            std::unique_lock<std::mutex> lock(engine->renderStartMutex);

            isRendering_ = true;
            engine->condSyncRender.wait(lock);
            renderLength_ = nextRenderLength_;
            nextRenderLength_ = 0;
            frameIndex = engine->frameIndex - 1;
        }
        //We only start the new graphics frame if the engine had time to loop
        if (engine->isRunning)
        {

            rmt_ScopedCPUSample(ActiveRenderLoop, 0);
            std::unique_lock<std::mutex> lock(renderingMutex);
            renderWindow_->setActive(true);
            renderWindow_->setView(views_[frameIndex % 2]);


            renderWindow_->clear(engine->config.bgColor);

            //manage command buffers
            auto& commandBuffer = commandBuffers_[frameIndex % 2];
#ifdef __neko_dbg__
            {
                std::ostringstream oss;
                oss << "Command Buffers length: " << renderLength_ << "\n";
                oss << "Engine frame: " << engine->frameIndex << " Graphics Frame: " << frameIndex;
                logDebug(oss.str());

            }
#endif
            for (auto i = 0u; i < renderLength_; i++)
            {
                std::unique_lock<std::mutex> renderLock(engine->renderStartMutex);
                auto* command = commandBuffer[i];
                if (command != nullptr)
                {
                    command->Draw(renderWindow_);
                }
            }
            editor->Update(engine->clockDeltatime.asSeconds());
            renderWindow_->display();
            renderWindow_->setActive(false);
        }

    }
    while (engine->isRunning);

    renderWindow_->setActive(true);
    logDebug("Graphics Loop Destroy");
    renderWindow_->setActive(false);


}

bool MultiThreadGraphicsManager::DidRenderingStart() const
{
    return isRendering_;
}

}