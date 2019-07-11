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

#include <vector>
#include <map>
#include <SFML/Graphics/Sprite.hpp>
#include "SFML/Graphics/Texture.hpp"
#include "texture.h"
#include <utilities/time_utility.h>
#include <utilities/json_utility.h>
#include <engine/system.h>

namespace neko
{
class MultiThreadSpriteManager;
/**
 * \brief 2d animation data used to switch between the textures at the given frequency, used in Init and Update
 */
struct SpriteAnimDef
{
    std::string name = "";
    int freq = 60;
    std::vector<int> imgIndexes;
    std::vector<sf::IntRect> imgRect; //allow to have subtexture
};
/**
 * \brief 2d animator description file used to initialize an animator, only used on Init
 */
struct SpriteAnimatorDef
{
    int spriteIndex = -1;
    std::vector<std::shared_ptr<sf::Texture>> textures;
};
/**
 * \brief manage an animator with several animations (SpriteAnimDef)
 */
class SpriteAnimator
{
public:
    void Init(json& animatorJson, TextureManager& textureManager, int spriteIndex);

    void Init(SpriteAnimatorDef& spriteAnimatorDef, SpriteAnimDef* animsDef, size_t animsCount);

    void PlayAnim(const std::string& animName);

    void PlayAnim(unsigned int animIndex);

    void Update(sf::Sprite* sprite, float dt);

    Index spriteIndex = 0;
private:
    std::vector<SpriteAnimDef> anims_;
    SpriteAnimDef* currentAnim_ = nullptr;
    Timer animTimer_ = {0.0f, 0.0f};
    Index currentIndex_ = 0u;
    std::vector<std::shared_ptr<sf::Texture>> textures_;
};
/**
 * \brief manages animators
 */
class AnimatorManager
{
public:
    AnimatorManager();
//TODO should return an index
    SpriteAnimator& CreateSpriteAnimator();

    void Update(MultiThreadSpriteManager& spriteManager, float dt);

    SpriteAnimator* GetAnimatorAt(unsigned int i);

private:
    std::vector<SpriteAnimator> animators_;
};
}
