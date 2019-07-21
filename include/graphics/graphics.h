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


/**
 * \brief abstraction of a graphic command send to the render thread
 */
struct Command
{
    int layer = 0;
    virtual void Draw(sf::RenderTarget* renderTarget) = 0;
};

/**
 * \brief specialization for SFML basic drawable type
 */
struct SfmlCommand : public Command
{
    /**
     * \brief non owning raw pointer of an SFML Drawable stored in a graphic class with double buffering (SpriteManager, ShapeManager, etc...)
     */
    sf::Drawable* drawable = nullptr;

    void Draw(sf::RenderTarget* renderTarget) override;
};
/**
 * \brief specialization for SFML vertex array drawing type
 */
struct TilemapCommand : public Command
{
    sf::Texture* texture = nullptr;
    sf::VertexArray* vertexArray = nullptr;

    void Draw(sf::RenderTarget* renderTarget) override;
};


class GraphicsManager
{
public:
	virtual ~GraphicsManager() {}
    virtual void Draw(sf::Drawable& drawable, int layer = 0) = 0;

    virtual void Draw(sf::VertexArray* vertexArray, sf::Texture* texture, int layer = 0) = 0;


protected:

    virtual void Draw(std::array<Command*, MAX_COMMAND_NMB>& commandBuffers_) = 0;
};

class BasicGraphicsManager : public GraphicsManager
{
public:
    virtual void Draw(sf::Drawable& drawable, int layer = 0);

    virtual void Draw(sf::VertexArray* vertexArray, sf::Texture* texture, int layer = 0);
	
protected:
    virtual void Draw(std::array<Command*, MAX_COMMAND_NMB>& commandBuffers_);

    std::array<Command*, MAX_COMMAND_NMB> commandBuffer_ = {};
    std::array<neko::SfmlCommand, neko::MAX_COMMAND_NMB> commands_;
    std::array<neko::TilemapCommand, neko::MAX_COMMAND_NMB> tileCommands_;
};

}
