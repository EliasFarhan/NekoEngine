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
#include <graphics/anim.h>
#include <cstring>
#include <engine/log.h>
#include <engine/engine.h>
#include <graphics/sprite.h>

namespace neko
{

void SpriteAnimator::Init(json& animatorJson, TextureManager& textureManager, int spriteIndex)
{
    SpriteAnimatorDef spriteAnimatorDef;
    std::vector<SpriteAnimDef> spriteAnimDefs;
    std::vector<std::shared_ptr<sf::Texture>> texturesTmp;

    for (auto& spriteAnimJson : animatorJson["anims"])
    {
        SpriteAnimDef spriteAnimDef{};
        spriteAnimDef.name = spriteAnimJson["name"];
        spriteAnimDef.freq = spriteAnimJson["freq"];
        for (auto& spriteAnimKeyJson : spriteAnimJson["keys"])
        {
            auto rect = GetRectFromJson(spriteAnimKeyJson, "rects");
            int textureKey = spriteAnimKeyJson["value"];
            spriteAnimDef.imgRect.push_back(rect);
            spriteAnimDef.imgIndexes.push_back(textureKey);

        }
        spriteAnimDefs.push_back(spriteAnimDef);
    }

    for (auto& texturePath : animatorJson["textures"])
    {
        std::string pathName = texturePath;
        auto textureIndex = textureManager.LoadTexture(pathName);
		std::weak_ptr<sf::Texture> texture = textureManager.GetTexture(textureIndex);
        texturesTmp.emplace_back(texture);
    }
    spriteAnimatorDef.textures.resize(texturesTmp.size());
    spriteAnimatorDef.textures.push_back(texturesTmp[0]);
    spriteAnimatorDef.spriteIndex = spriteIndex;
    Init(spriteAnimatorDef, &spriteAnimDefs[0], spriteAnimDefs.size());
}

void SpriteAnimator::Init(SpriteAnimatorDef& spriteAnimatorDef, SpriteAnimDef* animsDef, size_t animsCount)
{
    anims.resize(animsCount);
    for (unsigned int i = 0; i < animsCount; i++)
    {
        anims[i] = animsDef[i];
    }
    PlayAnim(0);
    spriteIndex = spriteAnimatorDef.spriteIndex;
    textures.resize(spriteAnimatorDef.textures.size());
	textures = spriteAnimatorDef.textures;
}

void SpriteAnimator::PlayAnim(const std::string& animName)
{
    for (unsigned int i = 0; i < anims.size(); i++)
    {
        auto& anim = anims[i];
        if (anim.name == animName)
        {
            PlayAnim(i);
            return;
        }
    }
    logDebug("Cannot play anim: " + animName);
}

void SpriteAnimator::PlayAnim(unsigned int animIndex)
{
    if (animIndex >= (unsigned int)anims.size())
    {
        logDebug("[Error] Invalid anim index");
        return;
    }
    auto& anim = anims[animIndex];
    animTimer.period = 1.0f / anim.freq;
    animTimer.Reset();
    currentIndex = 0;
    currentAnim = &anims[animIndex];
}

void SpriteAnimator::Update(sf::Sprite* sprite, float dt)
{
    if (currentAnim != nullptr)
    {
        animTimer.Update(dt);
        if (animTimer.IsOver())
        {
            currentIndex++;
            if (currentIndex >= static_cast<int>(currentAnim->imgIndexes.size()))
            {
                currentIndex = 0;
            }

            animTimer.Reset();
        }
        auto& texture = *textures[currentAnim->imgIndexes[currentIndex]];
        sprite->setTexture(texture);
        sprite->setTextureRect(currentAnim->imgRect[currentIndex]);
        sprite->setOrigin(sf::Vector2f(texture.getSize()) / 2.0f);
    }
}


AnimatorManager::AnimatorManager()
{
    animators.reserve(InitEntityNmb);
}

SpriteAnimator& AnimatorManager::CreateSpriteAnimator()
{
    SpriteAnimator newSpriteAnimator;
    animators.push_back(newSpriteAnimator);
    return animators.back();
}

void AnimatorManager::Update(SpriteManager& spriteManager, float dt)
{

    for (auto& animator : animators)
    {
        auto sprite = spriteManager.GetSpriteAt(animator.spriteIndex);
        animator.Update(sprite, dt);
    }
}

SpriteAnimator* AnimatorManager::GetAnimatorAt(unsigned int i)
{
    if (i >= animators.size())
        return nullptr;
    return &animators[i];
}

}