#include <graphics/graphics.h>
#include <engine/engine.h>
#include "SFML/Graphics/RenderTexture.hpp"
#include "engine/log.h"
#include <sstream>

namespace neko
{
void SfmlCommand::Draw(sf::RenderTarget* renderTarget)
{
	renderTarget->draw(*drawable);
}

void GraphicsManager::Draw(sf::Drawable& drawable)
{
	
	SfmlCommand sfmlCommand;
	sfmlCommand.drawable = &drawable;
	commands[(frameIndex + 1) % 2].push_back(sfmlCommand);
	commandBuffers[(frameIndex + 1) % 2].push(&commands[(frameIndex + 1) % 2].back());
}

void GraphicsManager::RenderLoop()
{
	auto* engine = MainEngine::GetInstance();
	renderWindow = engine->renderWindow;
    do
    {
        isReady = true;
		{
			std::unique_lock<std::mutex> lock(engine->renderMutex);
			engine->condSyncRender.notify_all();
		}
        isReady = false;
        renderWindow->setActive(true);
        if (engine->isRunning)
        {
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
}
}
