#include <graphics/graphics.h>
#include <engine/engine.h>
#include "SFML/Graphics/RenderTexture.hpp"
#include "engine/log.h"
#include <sstream>
#include <Remotery.h>
#include "engine/globals.h"

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
    if(nextRenderLength >= MAX_COMMAND_NMB)
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

void GraphicsManager::RenderLoop()
{
	auto* engine = MainEngine::GetInstance();
	
	renderWindow = engine->renderWindow;
	renderWindow->setActive(true);
	rmt_BindOpenGL();
	renderWindow->setActive(false);
    do
    {
		
        const bool isReady = engine->isReady;
		{
			/*{
				std::ostringstream oss;
				oss << "Graphics Frame Start: " << MainEngine::GetInstance()->frameIndex << " and Graphics frame: " << frameIndex;
				logDebug(oss.str());
			}*/
			std::unique_lock<std::mutex> lock(engine->renderMutex);
			renderLength = nextRenderLength;
			nextRenderLength = 0;
			engine->condSyncRender.notify_all();
		}
        renderWindow->setActive(true);
		//We only start the new graphics frame if the engine had time to loop
        if (engine->isRunning && isReady)
        {
			rmt_ScopedCPUSample(RenderLoop, 0);
			rmt_ScopedOpenGLSample(RenderLoop);
			isRendering = true;
            {
                //we need to lock the render thread while drawing
                std::unique_lock<std::mutex> lock(engine->renderMutex);
                renderWindow->clear(engine->config.bgColor);

                //manage command buffers
                auto &commandBuffer = commandBuffers[frameIndex % 2];
                for(auto i = 0u; i<renderLength;i++)
                {
                    auto *command = commandBuffer[i];
                    command->Draw(renderWindow);
                }
            }
			renderWindow->display();
			
			
        	isRendering = false;
        	++frameIndex;

        }
	
		renderWindow->setActive(false);
		
	} while (engine->isRunning);

	renderWindow->setActive(true);
	logDebug("Graphics Loop Destroy");
	rmt_UnbindOpenGL();
	renderWindow->setActive(false);

	engine->condSyncRender.notify_all();

}
}
