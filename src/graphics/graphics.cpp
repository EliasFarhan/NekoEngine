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
        condSyncRender.wait_for(lock, 0.5ms);
        if (engine->isRunning)
        {
			isRendering = true;
            renderTarget->clear(sf::Color::Black);
            
        	//TODO manage command buffers
			while(!commandBuffer.empty())
			{
				auto* command = commandBuffer.front();
				commandBuffer.pop();
				command->Draw(renderTarget);
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
        }
	} while (engine->isRunning);
}
}
