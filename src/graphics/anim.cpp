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
    SpriteAnimatorDef spriteAnimatorDef{};
    std::vector<SpriteAnimDef> spriteAnimDefs;
    std::vector<std::shared_ptr<sf::Texture>> texturesTmp;

    for (auto& spriteAnimJson : animatorJson["anims"])
    {
        SpriteAnimDef spriteAnimDef{};
        spriteAnimDef.name = spriteAnimJson["name"];
        spriteAnimDef.freq = spriteAnimJson["freq"];
        for (auto& spriteAnimKeyJson : spriteAnimJson["keys"])
        {
            auto rect = GetIntRectFromJson(spriteAnimKeyJson, "rects");
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
		auto* texture = textureManager.GetTexture(textureIndex);
        spriteAnimatorDef.textures.push_back(*texture);
    }
    spriteAnimatorDef.spriteIndex = spriteIndex;
    Init(spriteAnimatorDef, &spriteAnimDefs[0], spriteAnimDefs.size());
}

void SpriteAnimator::Init(SpriteAnimatorDef& spriteAnimatorDef, SpriteAnimDef* animsDef, size_t animsCount)
{
    anims_.resize(animsCount);
    for (unsigned int i = 0; i < animsCount; i++)
    {
        anims_[i] = animsDef[i];
    }
    PlayAnim(0);
    spriteIndex = spriteAnimatorDef.spriteIndex;
    textures_.resize(spriteAnimatorDef.textures.size());
	textures_ = spriteAnimatorDef.textures;
}

void SpriteAnimator::PlayAnim(const std::string& animName)
{
    for (unsigned int i = 0; i < anims_.size(); i++)
    {
        auto& anim = anims_[i];
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
    if (animIndex >= (unsigned int)anims_.size())
    {
        logDebug("[Error] Invalid anim index");
        return;
    }
    auto& anim = anims_[animIndex];
    animTimer_.period = 1.0f / anim.freq;
    animTimer_.Reset();
    currentIndex_ = 0;
    currentAnim_ = &anims_[animIndex];
}

void SpriteAnimator::Update(sf::Sprite* sprite, float dt)
{
    if (currentAnim_ != nullptr)
    {
        animTimer_.Update(dt);
        if (animTimer_.IsOver())
        {
            currentIndex_++;
            if (currentIndex_ >= static_cast<int>(currentAnim_->imgIndexes.size()))
            {
                currentIndex_ = 0;
            }

            animTimer_.Reset();
        }

        auto& texture = textures_[currentAnim_->imgIndexes[currentIndex_]];
        sprite->setTexture(texture);
        sprite->setTextureRect(currentAnim_->imgRect[currentIndex_]);
        sprite->setOrigin(sf::Vector2f(texture.getSize()) / 2.0f);

    }
}


AnimatorManager::AnimatorManager()
{
    animators_.reserve(INIT_ENTITY_NMB);
}

SpriteAnimator& AnimatorManager::CreateSpriteAnimator()
{
    SpriteAnimator newSpriteAnimator;
    animators_.push_back(newSpriteAnimator);
    return animators_.back();
}

void AnimatorManager::Update(SpriteManager& spriteManager, float dt)
{

    for (auto& animator : animators_)
    {
        auto* sprite = spriteManager.GetSpriteAt(animator.spriteIndex);
		if (sprite != nullptr)
		{
			animator.Update(sprite, dt);
		}
    }
}

SpriteAnimator* AnimatorManager::GetAnimatorAt(unsigned int i)
{
    if (i >= animators_.size())
        return nullptr;
    return &animators_[i];
}

}