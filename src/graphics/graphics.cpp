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
    if (nextRenderLength >= MAX_COMMAND_NMB)
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
    commands[index][nextRenderLength] = command;
    commandBuffers[index][nextRenderLength] = &commands[index][nextRenderLength];

    nextRenderLength++;
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

    tileCommands[index][nextRenderLength] = tilemapCommand;
    commandBuffers[index][nextRenderLength] = &tileCommands[index][nextRenderLength];

    nextRenderLength++;
}

void GraphicsManager::RenderLoop()
{
    auto* engine = MainEngine::GetInstance();

    renderWindow = engine->renderWindow;
    renderWindow->setActive(true);
    editor.graphicsManager = this;
    editor.renderWindow = renderWindow;
    renderWindow->setActive(false);
    do
    {
        rmt_ScopedCPUSample(RenderLoop, 0);
        if(engine->isReady)
        {
            /*{
                std::ostringstream oss;
                oss << "Graphics Frame Start: " << MainEngine::GetInstance()->frameIndex << " and Graphics frame: " << frameIndex;
                logDebug(oss.str());
            }*/
            std::unique_lock<std::mutex> lock(engine->renderMutex);
            renderLength = nextRenderLength;
            nextRenderLength = 0;
            frameIndex = engine->frameIndex-1;
            engine->condSyncRender.notify_all();
        }
        else
        {
            continue;
        }
        //We only start the new graphics frame if the engine had time to loop
        if (engine->isRunning)
        {
            rmt_ScopedCPUSample(ActiveRenderLoop, 0);
            renderWindow->setActive(true);

            isRendering = true;


            renderWindow->clear(engine->config.bgColor);

            //manage command buffers
            auto& commandBuffer = commandBuffers[frameIndex % 2];
#ifdef __neko_dbg__
            {
                std::ostringstream oss;
                oss << "Command Buffers length: "<<renderLength<<"\n";
                oss << "Engine frame: "<< engine->frameIndex <<" Graphics Frame: "<< frameIndex;
                logDebug(oss.str());

            }
#endif
            for (auto i = 0u; i < renderLength; i++)
            {
                std::unique_lock<std::mutex> lock(engine->renderMutex);
                auto* command = commandBuffer[i];
                if(command != nullptr)
                    command->Draw(renderWindow);
            }
            editor.Update();
            renderWindow->display();


            isRendering = false;


        }

        renderWindow->setActive(false);

    }
    while (engine->isRunning);

    renderWindow->setActive(true);
    logDebug("Graphics Loop Destroy");
    renderWindow->setActive(false);

    engine->condSyncRender.notify_all();

}


}
