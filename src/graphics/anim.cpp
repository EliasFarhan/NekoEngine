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
    std::vector<sf::Texture*> texturesTmp;

    for(auto& spriteAnimJson : animatorJson["anims"])
    {
        SpriteAnimDef spriteAnimDef{};
        spriteAnimDef.name = spriteAnimJson["name"];
        spriteAnimDef.freq = spriteAnimJson["freq"];
        for(auto& spriteAnimKeyJson : spriteAnimJson["keys"])
        {
            auto rect = GetRectFromJson(spriteAnimKeyJson, "rects");
            int textureKey = spriteAnimKeyJson["value"];
            spriteAnimDef.imgRect.push_back(rect);
            spriteAnimDef.imgIndexes.push_back(textureKey);

        }
        spriteAnimDefs.push_back(spriteAnimDef);
    }

    for(auto& texturePath : animatorJson["textures"])
    {
        std::string pathName = texturePath;
        auto* texture = textureManager.LoadTexture(pathName);
        texturesTmp.push_back(texture);
    }
    spriteAnimatorDef.textureCount = texturesTmp.size();
    spriteAnimatorDef.textures = &texturesTmp[0];
    spriteAnimatorDef.spriteIndex = spriteIndex;
    Init(spriteAnimatorDef, &spriteAnimDefs[0], spriteAnimDefs.size());
}
void SpriteAnimator::Init(SpriteAnimatorDef& spriteAnimatorDef, SpriteAnimDef* animsDef, size_t animsCount)
{
    anims.resize(animsCount);
    for(int i = 0; i < animsCount; i++)
    {
        anims[i] = animsDef[i];
    }
    PlayAnim(0);
    spriteIndex = spriteAnimatorDef.spriteIndex;
    textures.resize(spriteAnimatorDef.textureCount);
    memcpy(&textures[0], spriteAnimatorDef.textures, spriteAnimatorDef.textureCount*sizeof(sf::Texture*));
}

void SpriteAnimator::PlayAnim(const std::string &animName)
{
    for(int i = 0; i < anims.size();i++)
    {
        auto& anim = anims[i];
        if(anim.name == animName)
        {
            PlayAnim(i);
            return;
        }
    }
    logDebug("Cannot play anim: "+animName);
}

void SpriteAnimator::PlayAnim(int animIndex)
{
    if(animIndex >= anims.size())
    {
        logDebug("[Error] Invalid anim index");
        return;
    }
    auto& anim = anims[animIndex];
    animTimer.period = 1.0f/anim.freq;
    animTimer.Reset();
    currentIndex = 0;
    currentAnim = &anims[animIndex];
}

void SpriteAnimator::Update(sf::Sprite* sprite, float dt)
{
    if(currentAnim != nullptr)
    {
        animTimer.Update(dt);
        if(animTimer.IsOver())
        {
            currentIndex++;
            if(currentIndex >= currentAnim->imgIndexes.size())
            {
                currentIndex = 0;
            }

            animTimer.Reset();
        }
        auto& texture = *textures[currentAnim->imgIndexes[currentIndex]];
        sprite->setTexture(texture);
        sprite->setTextureRect(currentAnim->imgRect[currentIndex]);
        sprite->setOrigin(sf::Vector2f(texture.getSize())/2.0f);
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

void AnimatorManager::Update(SpriteManager &spriteManager, float dt)
{

    for(auto& animator : animators)
    {
        auto sprite = spriteManager.GetSpriteAt(animator.spriteIndex);
        animator.Update(sprite, dt);
    }
}

SpriteAnimator *AnimatorManager::GetAnimatorAt(int i)
{
    if(i >= animators.size())
        return nullptr;
    return &animators[i];
}

}