/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <graphics/graphics.h>
#include <engine/engine.h>
#include "SFML/Graphics/RenderTexture.hpp"
#include "engine/log.h"
#include <sstream>
#include <Remotery.h>
#include "engine/globals.h"
#include <cstdio>

namespace neko
{
void SfmlCommand::Draw(sf::RenderTarget* renderTarget)
{
    renderTarget->draw(*drawable);
}

GraphicsManager::GraphicsManager()
{
}

void GraphicsManager::Draw(sf::Drawable& drawable)
{
    if (nextRenderLength_ >= MAX_COMMAND_NMB)
    {
        logDebug("[Error] Too many draw calls compare to MAX_COMMAND_NMB");
        return;
    }
    const int index = MainEngine::GetInstance()->frameIndex % 2;
    /*{
        std::ostringstream oss;
        oss << "Graphics Command On Engine Frame: " << MainEngine::GetInstance()->frameIndex <<" and Graphics frame: "<<frameIndex;
        logDebug(oss.str());
    }*/
    SfmlCommand command;
    command.drawable = &drawable;
    commands_[index][nextRenderLength_] = command;
    commandBuffers_[index][nextRenderLength_] = &commands_[index][nextRenderLength_];

    nextRenderLength_++;
}


void TilemapCommand::Draw(sf::RenderTarget* renderTarget)
{
    sf::RenderStates states;
    states.texture = texture;
    renderTarget->draw(*vertexArray, states);
}


void GraphicsManager::Draw(sf::VertexArray* vertexArray, sf::Texture* texture)
{
    const int index = MainEngine::GetInstance()->frameIndex % 2;
    TilemapCommand tilemapCommand;
    tilemapCommand.vertexArray = vertexArray;
    tilemapCommand.texture = texture;

    tileCommands_[index][nextRenderLength_] = tilemapCommand;
    commandBuffers_[index][nextRenderLength_] = &tileCommands_[index][nextRenderLength_];

    nextRenderLength_++;
}

void GraphicsManager::SetView(sf::View view)
{
    const int index = MainEngine::GetInstance()->frameIndex % 2;
    views_[index] = view;
}

void GraphicsManager::RenderLoop()
{
    auto* engine = MainEngine::GetInstance();

    renderWindow_ = engine->renderWindow.get();
    renderWindow_->setActive(true);
    views_[0] = renderWindow_->getView();
    views_[1] = views_[0];
    editor.graphicsManager_ = this;
    editor.renderWindow_ = renderWindow_;
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
                std::unique_lock<std::mutex> lock(engine->renderStartMutex);
                auto* command = commandBuffer[i];
                if (command != nullptr)
                    command->Draw(renderWindow_);
            }
            editor.Update();
            renderWindow_->display();
            renderWindow_->setActive(false);
        }

    }
    while (engine->isRunning);

    renderWindow_->setActive(true);
    logDebug("Graphics Loop Destroy");
    renderWindow_->setActive(false);


}


}
