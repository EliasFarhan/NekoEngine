#pragma once


#include <mutex>
#include <array>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/View.hpp>

#include <engine/editor.h>
#include <graphics/graphics.h>

namespace multi
{
/**
 * \brief graphics manager run in a render thread by the MainEngine
 */
class GraphicsManager : public neko::GraphicsManager
{
public:
    GraphicsManager();

/**
 * \brief called by the render loop when iterating through all the basic sfml commands
 * Should not be called from engine thread
 * @param drawable
 */
    virtual void Draw(sf::Drawable& drawable, int layer = 0) override;
/**
 * \brief called by the render loop when iterating through all the tilemap commands
 * Should not be called from engine thread
 * @param vertexArray
 * @param texture
 */
    virtual void Draw(sf::VertexArray* vertexArray, sf::Texture* texture, int layer = 0) override;
/**
 * \brief called from engine loop, changing the view for the next frame
 * @param view
 */
    virtual void SetView(sf::View view);
/**
 * \brief run by MainEngine in the render thread
 */
    virtual void RenderLoop();

    bool DidRenderingStart() const;

public:
    std::unique_ptr<neko::Editor> editor = nullptr;
    //Used for Engine loop to wait for graphics thread
    std::mutex renderingMutex;
protected:
    void Draw(std::array<neko::Command*, neko::MAX_COMMAND_NMB>& commandBuffers_) override;

    /**
     * \brief non owning ptr to renderwindow
     */
    sf::RenderWindow* renderWindow_ = nullptr;
    std::array<neko::SfmlCommand, neko::MAX_COMMAND_NMB> commands_[2];
    std::array<neko::TilemapCommand, neko::MAX_COMMAND_NMB> tileCommands_[2];
    std::array<neko::Command*, neko::MAX_COMMAND_NMB> commandBuffers_[2];
    sf::View views_[2];
    size_t renderLength_ = 0;
    size_t nextRenderLength_ = 0;
    neko::Index frameIndex = 0u;
    bool isRendering_ = false;
};
}