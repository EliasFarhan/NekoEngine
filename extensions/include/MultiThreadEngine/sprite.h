#pragma once

namespace multi
{


MultiThreadSpriteManager::MultiThreadSpriteManager()
{
    sprites_[0].reserve(INIT_ENTITY_NMB);
    sprites_[1].reserve(INIT_ENTITY_NMB);
}

Index MultiThreadSpriteManager::AddSprite(const std::shared_ptr<sf::Texture> texture)
{
    sf::Sprite sprite;
    if (texture.get() != nullptr)
    {
        sprite.setTexture(*texture);
        sprite.setOrigin(sf::Vector2f(texture->getSize()) / 2.0f);
    }
    const Index index = Index(sprites_[0].size());
    sprites_[0].push_back(sprite);
    sprites_[1].push_back(sprite);
    return index;
}

void MultiThreadSpriteManager::CopyTransformPosition(OldTransform2dManager& transformManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpritePositions, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto i = start; i < start + length; i++)
    {
        sprites_[frameIndex][i].setPosition(transformManager.positions_[i]);
    }
}

void MultiThreadSpriteManager::CopyTransformScales(OldTransform2dManager& transformManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpriteScales, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto i = start; i < start + length; i++)
    {
        sprites_[frameIndex][i].setScale(transformManager.scales_[i]);
    }
}

void MultiThreadSpriteManager::CopyTransformAngles(OldTransform2dManager& transformManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpriteAngles, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto i = start; i < start + length; i++)
    {
        sprites_[frameIndex][i].setRotation(transformManager.angles_[i]);
    }
}

void MultiThreadSpriteManager::PushCommands(GraphicsManager* graphicsManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(PushSpriteCommands, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto i = start; i < start + length; i++)
    {
        graphicsManager->Draw(sprites_[frameIndex][i]);
    }
}

sf::Sprite* MultiThreadSpriteManager::GetSpriteAt(unsigned int spriteIndex)
{
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
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