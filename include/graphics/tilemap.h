#pragma once


#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <physics/physics.h>

namespace neko
{
class Tilemap
{
public:

};

class TiledSheet
{
    sf::Texture *textures;
    sf::VertexArray tilemap[2];
};

class TiledMap
{
public:
    void Init(const std::string &tilemapPath, Physics2dManager* physics2DManager = nullptr);

protected:
    std::vector<TiledSheet> tilesheets;

};
}