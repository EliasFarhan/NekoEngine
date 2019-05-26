#include <graphics/graphics.h>
#include <engine/engine.h>
#include "SFML/Graphics/RenderTexture.hpp"
#include "engine/log.h"
#include <sstream>
#include <Remotery.h>

namespace neko
{
void SfmlCommand::Draw(sf::RenderTarget* renderTarget)
{
	renderTarget->draw(*drawable);
}

void GraphicsManager::Draw(sf::Drawable& drawable)
{

	const int index = MainEngine::GetInstance()->frameIndex % 2;
	{
		std::ostringstream oss;
		oss << "Graphics Command On Engine Frame: " << index <<" and Graphics frame: "<<frameIndex;
		logDebug(oss.str());
	}
	SfmlCommand sfmlCommand;
	sfmlCommand.drawable = &drawable;
	commands[index].push_back(sfmlCommand);
	commandBuffers[index].push(&commands[index].back());
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
		rmt_ScopedCPUSample(RenderLoop, 0);
        isReady = true;
		{
			/*{
				std::ostringstream oss;
				oss << "Graphics Frame Start: " << MainEngine::GetInstance()->frameIndex << " and Graphics frame: " << frameIndex;
				logDebug(oss.str());
			}*/
			std::unique_lock<std::mutex> lock(engine->renderMutex);
			engine->condSyncRender.notify_all();
		}
        isReady = false;
        renderWindow->setActive(true);
        if (engine->isRunning)
        {

			rmt_ScopedOpenGLSample(RenderLoop);
			isRendering = true;
            renderWindow->clear(sf::Color::Black);
            
        	//manage command buffers
			auto& commandBuffer = commandBuffers[frameIndex % 2];
			while(!commandBuffer.empty())
			{
				auto* command = commandBuffer.front();
				command->Draw(renderWindow);
				commandBuffer.pop();
			}

			renderWindow->display();
			
			
        	isRendering = false;
			commands[frameIndex % 2].clear();
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
