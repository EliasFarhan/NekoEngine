#pragma once
#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <graphics/tilemap.h>

namespace neko
{
class GraphicsManager;
}

namespace multi
{
/**
 * \brief simple abstraction for one tilesheet with a tilemap texture with a double buffered vertex array
 */
struct Tilesheet
{
    std::shared_ptr<sf::Texture> texture = nullptr;
    /**
     * \brief vertex array with 2 vectors for double buffering with the render thread
     */
    sf::VertexArray tilemap[2];
};


/**
 * \brief Abstraction of Tiled (https://www.mapeditor.org/) tilesheets
 */
struct Tiledsheet : public Tilesheet
{
    sf::Vector2u tileSize;
    sf::Vector2u size;
    unsigned firstId;
    unsigned tileNmb;
};

/*
* Abstraction of Tiled tilemap with different tilesheets with their own vertex arrays
*/
class TiledMap : public neko::Tilemap
{
public:
    void
    Init(const std::string& tilemapPath, neko::TextureManager& textureManager, neko::Physics2dManager* physics2DManager = nullptr);
    void PushCommand(neko::GraphicsManager* graphicsManager) override;
protected:
    std::vector<Tiledsheet> tileSheets_;

};
}