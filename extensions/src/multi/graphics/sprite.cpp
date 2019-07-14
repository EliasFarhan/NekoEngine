#include <multi/graphics/sprite.h>
#include <SFML/Graphics/Texture.hpp>
#include <Remotery.h>
#include <multi/engine/engine.h>
#include <engine/transform.h>

namespace multi
{


SpriteManager::SpriteManager()
{
    sprites_[0].reserve(neko::INIT_ENTITY_NMB);
    sprites_[1].reserve(neko::INIT_ENTITY_NMB);
}

neko::Index SpriteManager::AddSprite(const std::shared_ptr<sf::Texture> texture)
{
    sf::Sprite sprite;
    if (texture.get() != nullptr)
    {
        sprite.setTexture(*texture);
        sprite.setOrigin(sf::Vector2f(texture->getSize()) / 2.0f);
    }
    const neko::Index index = neko::Index(sprites_[0].size());
    sprites_[0].push_back(sprite);
    sprites_[1].push_back(sprite);
    return index;
}

void SpriteManager::CopyTransformPosition(neko::Position2dManager& transformManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpritePositions, 0);
    const int frameIndex = MainEngine::GetFrameIndex() % 2;
    for (auto i = start; i < start + length; i++)
    {
        sprites_[frameIndex][i].setPosition(transformManager.GetComponent(i));
    }
}

void SpriteManager::CopyTransformScales(neko::Scale2dManager& transformManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpriteScales, 0);
    const int frameIndex = MainEngine::GetFrameIndex() % 2;
    for (auto i = start; i < start + length; i++)
    {
        sprites_[frameIndex][i].setScale(transformManager.GetComponent(i));
    }
}

void SpriteManager::CopyTransformAngles(neko::Angle2dManager& transformManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpriteAngles, 0);
    const int frameIndex = MainEngine::GetFrameIndex() % 2;
    for (auto i = start; i < start + length; i++)
    {
        sprites_[frameIndex][i].setRotation(transformManager.GetComponent(i));
    }
}

void SpriteManager::PushCommands(neko::GraphicsManager* graphicsManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(PushSpriteCommands, 0);
    const int frameIndex = MainEngine::GetFrameIndex()% 2;
    for (auto i = start; i < start + length; i++)
    {
        graphicsManager->Draw(sprites_[frameIndex][i]);
    }
}

sf::Sprite* SpriteManager::GetSpriteAt(unsigned int spriteIndex)
{
    const int frameIndex = MainEngine::GetFrameIndex() % 2;
    if (spriteIndex >= sprites_[frameIndex].size())
    {
        return nullptr;
    }
    else
    {
        return &sprites_[frameIndex][spriteIndex];
    }
}

}