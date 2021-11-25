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

    /**
     * \brief graphics manager run in a render thread by the MainEngine
     */
    class GraphicsManager
    {
    public:
        GraphicsManager();

        /**
         * \brief called by the render loop when iterating through all the basic sfml commands
         * Should not be called from engine thread
         * @param drawable
         */
        virtual void Draw(sf::Drawable& drawable);
        /**
         * \brief called by the render loop when iterating through all the tilemap commands
         * Should not be called from engine thread
         * @param vertexArray
         * @param texture
         */
        virtual void Draw(sf::VertexArray* vertexArray, sf::Texture* texture);
        /**
         * \brief called from engine loop, changing the view for the next frame
         * @param view
         */
        virtual void SetView(sf::View view);

        bool DidRenderingStart() const;
        std::unique_ptr<Editor> editor = nullptr;
        //Used for Engine loop to wait for graphics thread
        std::mutex renderingMutex;

        void Update();
        void Init();
    protected:
        /**
         * \brief non owning ptr to renderwindow
         */
        sf::RenderWindow* renderWindow_ = nullptr;
        std::array<SfmlCommand, MAX_COMMAND_NMB> commands_[2];
        std::array<TilemapCommand, MAX_COMMAND_NMB> tileCommands_[2];
        std::array<Command*, MAX_COMMAND_NMB> commandBuffers_[2];
        sf::View views_[2];
        size_t renderLength_ = 0;
        size_t nextRenderLength_ = 0;
        Index frameIndex = 0u;
        bool isRendering_ = false;
    };
}
