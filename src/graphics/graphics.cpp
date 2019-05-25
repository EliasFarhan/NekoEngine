#include <graphics/graphics.h>
#include <engine/engine.h>
#include "SFML/Graphics/RenderTexture.hpp"

namespace neko
{
void SfmlCommand::Draw(sf::RenderTarget* renderTarget)
{
	renderTarget->draw(*drawable);
}

void GraphicsManager::Draw(sf::Drawable& drawable)
{
	//TODO schedule the command for the draw render loop

	//We have to wait for the previous frame rendering to be over
	std::unique_lock<std::mutex> lock(engine->renderMutex);

	SfmlCommand sfmlCommand;
	sfmlCommand.drawable = &drawable;
	commands[(frameIndex-1) % 2].push_back(sfmlCommand);
	commandBuffers[(frameIndex - 1) % 2].push(&commands[(frameIndex - 1) % 2].back());
}

void GraphicsManager::RenderLoop(Engine* engine, 
	std::condition_variable &condSyncRender,
	std::mutex &renderMutex)
{
	this->engine = engine;
    auto* renderTarget = engine->renderTarget;
    renderTarget->setActive(true);
    do
    {
        std::unique_lock<std::mutex> lock(renderMutex);

        using namespace std::chrono_literals;
        condSyncRender.wait(lock);
        if (engine->isRunning)
        {
			isRendering = true;
            renderTarget->clear(sf::Color::Black);
            
        	//TODO manage command buffers
			auto& commandBuffer = commandBuffers[frameIndex % 2];
			while(!commandBuffer.empty())
			{
				auto* command = commandBuffer.front();
				command->Draw(renderTarget);
				commandBuffer.pop();
			}

			auto* renderWindow = dynamic_cast<sf::RenderWindow*>(renderTarget);
			if (renderWindow != nullptr)
			{
				renderWindow->display();
			}
			auto* renderTexture= dynamic_cast<sf::RenderTexture*>(renderTarget);
			if (renderTexture != nullptr)
			{
				renderTexture->display();
			}
			
        	isRendering = false;
			commands[frameIndex % 2].clear();
        	++frameIndex;
			
        }
	} while (engine->isRunning);
}
}
