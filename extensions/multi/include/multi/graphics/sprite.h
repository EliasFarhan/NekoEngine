#pragma once

#include <graphics/sprite.h>

namespace neko
{
class GraphicsManager;
}
namespace multi
{

/**
* \brief store the sfml sprite and allow to copy transform from Transform2dManager,
* but transform need to be at the same index as sprite in an Entity Component System way
*/
class SpriteManager : public neko::SpriteManager
{
public:
    SpriteManager();

    neko::Index AddSprite(const sf::Texture* texture) override;

    sf::Sprite* GetSpriteAt(unsigned int spriteIndex) override ;

    /**
* \brief push basic graphic command to the render thread to be processed next frame
* @param graphicsManager
* @param start
* @param length
*/
    void PushCommands(neko::GraphicsManager* graphicsManager, size_t start = 0, size_t length = neko::INIT_ENTITY_NMB) override ;

    void CopyTransformPosition(neko::Position2dManager& transformManager, size_t start, size_t length) override;

    void CopyTransformScales(neko::Scale2dManager& transformManager, size_t start, size_t length) override;

    void CopyTransformAngles(neko::Angle2dManager& transformManager, size_t start, size_t length) override;

private:
    /**
 * \brief store the sfml sprites with two vectors for double buffering with the render thread
 */
    std::vector<sf::Sprite> sprites_[2];
};


}