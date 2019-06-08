#pragma once

/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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

#include <SFML/Graphics/RenderWindow.hpp>
#include <condition_variable>
#include <array>
#include <atomic>
#include <engine/editor.h>
#include "tilemap.h"

namespace neko
{

const size_t MAX_COMMAND_NMB = 1024;

class MainEngine;

struct Command
{
    virtual void Draw(sf::RenderTarget* renderTarget) = 0;
};

struct SfmlCommand : public Command
{
    sf::Drawable* drawable = nullptr;

    void Draw(sf::RenderTarget* renderTarget) override;
};

struct TilemapCommand : public Command
{
    sf::Texture* texture = nullptr;
    sf::VertexArray* vertexArray = nullptr;
    void Draw(sf::RenderTarget* renderTarget) override ;
};

class GraphicsManager
{
public:
    GraphicsManager();

    std::atomic<bool> isRendering = false;
    std::atomic<bool> isReady = false;

    std::atomic<unsigned int> frameIndex = 0;

    virtual void Draw(sf::Drawable& drawable);
    virtual void Draw(sf::VertexArray* vertexArray, sf::Texture* texture);
	virtual void SetView(sf::View view);
    virtual void RenderLoop();

    Editor editor;
protected:
    sf::RenderWindow* renderWindow = nullptr;
    std::array<SfmlCommand, MAX_COMMAND_NMB> commands[2];
    std::array<TilemapCommand, MAX_COMMAND_NMB> tileCommands[2];
    std::array<Command*, MAX_COMMAND_NMB> commandBuffers[2];
	sf::View views[2];
    size_t renderLength = 0;
    size_t nextRenderLength = 0;
};
}
