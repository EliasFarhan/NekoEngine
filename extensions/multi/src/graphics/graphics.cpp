#include <multi/graphics/graphics.h>
#include <multi/graphics/editor.h>
#include <engine/engine.h>
#include <multi/engine/engine.h>
#include <engine/log.h>
#include <Remotery.h>
#include <sstream>
namespace multi
{


GraphicsManager::GraphicsManager()
{
    commandBuffers_[0].fill(nullptr);
    commandBuffers_[1].fill(nullptr);
    editor = std::make_unique<multi::Editor>();
}

void GraphicsManager::Draw(sf::Drawable& drawable, int layer)
{
    if (nextRenderLength_ >= neko::MAX_COMMAND_NMB)
    {
        logDebug("[Error] Too many draw calls compare to MAX_COMMAND_NMB");
        return;
    }
    const neko::Index index = MainEngine::GetFrameIndex()  % 2;
    /*{
        std::ostringstream oss;
        oss << "Graphics Command On Engine Frame: " << MainEngine::GetInstance()->frameIndex <<" and Graphics frame: "<<frameIndex<<" with render length: "<< nextRenderLength_;
        logDebug(oss.str());
    }*/
    neko::SfmlCommand command;
    command.drawable = &drawable;
    command.layer = layer;
    commands_[index][nextRenderLength_] = command;
    commandBuffers_[index][nextRenderLength_] = &commands_[index][nextRenderLength_];

    nextRenderLength_++;
}


void GraphicsManager::Draw(sf::VertexArray* vertexArray, sf::Texture* texture, int layer)
{
    const neko::Index index = MainEngine::GetFrameIndex()  % 2;
    neko::TilemapCommand tilemapCommand;
    tilemapCommand.vertexArray = vertexArray;
    tilemapCommand.texture = texture;
    tilemapCommand.layer = layer;
    tileCommands_[index][nextRenderLength_] = tilemapCommand;
    commandBuffers_[index][nextRenderLength_] = &tileCommands_[index][nextRenderLength_];

    nextRenderLength_++;
}

void GraphicsManager::SetView(sf::View view)
{
    const neko::Index index = MainEngine::GetFrameIndex() % 2;
    views_[index] = view;
}

void GraphicsManager::RenderLoop()
{
    auto* engine = neko::BasicEngine::GetInstance<MainEngine>();

    renderWindow_ = engine->renderWindow.get();
    renderWindow_->setActive(true);
    views_[0] = renderWindow_->getView();
    views_[1] = views_[0];
    renderWindow_->setActive(false);

    do
    {
        rmt_ScopedCPUSample(RenderLoop, 0);
        neko::Index index = 0;

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
            index = MainEngine::GetFrameIndex() % 2;
            frameIndex = (index - 1)%2;
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
                oss << "Engine frame: " << index << " Graphics Frame: " << frameIndex;
                logDebug(oss.str());

            }
#endif
            Draw(commandBuffer);
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

bool GraphicsManager::DidRenderingStart() const
{
    return isRendering_;
}

void GraphicsManager::Draw(std::array<neko::Command*, neko::MAX_COMMAND_NMB>& commandBuffers_)
{
    auto* engine = neko::BasicEngine::GetInstance<MainEngine>();
    for (auto i = 0u; i < renderLength_; i++)
    {
        std::unique_lock<std::mutex> renderLock(engine->renderStartMutex);
        auto* command = commandBuffers_[i];
        if (command != nullptr)
        {
            command->Draw(renderWindow_);
        }
    }
}

}