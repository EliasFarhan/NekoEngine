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

namespace neko
{
void SfmlCommand::Draw(sf::RenderTarget* renderTarget)
{

    renderTarget->draw(*drawable, states);
}


void TilemapCommand::Draw(sf::RenderTarget* renderTarget)
{
    states.texture = texture;
    renderTarget->draw(*vertexArray, states);
}

//TODO Generate render command
void GraphicsManager::Draw(sf::Drawable& drawable, int layer, const sf::RenderStates& states)
{
	SfmlCommand sfmlCommand;
	sfmlCommand.drawable = &drawable;
	sfmlCommand.layer = layer;
    sfmlCommand.states = states;
	sfmlCommands[renderLength] = sfmlCommand;
	commandBuffer_[renderLength] = &sfmlCommands[renderLength];
	renderLength++;
}
//TODO Generate render command
void GraphicsManager::Draw(sf::VertexArray* vertexArray, sf::Texture* texture, int layer, const sf::RenderStates& states)
{

}

void GraphicsManager::Render(sf::RenderTarget* renderTarget)
{
	std::sort(commandBuffer_.begin(), commandBuffer_.begin() + renderLength, [](Command* c1, Command* c2)
	{
		return c1->layer < c2->layer;
	});
	for (size_t i = 0; i < renderLength; i++)
	{
		commandBuffer_[i]->Draw(renderTarget);
	}
	renderLength = 0;
}

}
