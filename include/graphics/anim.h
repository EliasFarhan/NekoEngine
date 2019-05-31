#pragma once
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
class SpriteManager;

struct SpriteAnimDef
{
	std::string name = "";
	int freq = 60;
	std::vector<int> imgIndexes;
	std::vector<sf::IntRect> imgRect; //allow to have subtexture
};

struct SpriteAnimatorDef
{
    int spriteIndex = -1;
    sf::Texture** textures = nullptr;
    size_t textureCount = 0;
};

class SpriteAnimator
{
public:
    void Init(json& animatorJson, TextureManager& textureManager, int spriteIndex);
    void Init(SpriteAnimatorDef& spriteAnimatorDef, SpriteAnimDef* animsDef, size_t animsCount);
    void PlayAnim(const std::string& animName);
    void PlayAnim(int animIndex);
    void Update(sf::Sprite* sprite, float dt);
    int spriteIndex = 0;
private:
    std::vector<SpriteAnimDef> anims;
    SpriteAnimDef* currentAnim = nullptr;
    Timer animTimer = {0.0f,0.0f};
    int currentIndex = 0;
    std::vector<sf::Texture*> textures;
};

class AnimatorManager
{
public:
    AnimatorManager();
    SpriteAnimator& CreateSpriteAnimator();
    void Update(SpriteManager& spriteManager, float dt);

    SpriteAnimator *GetAnimatorAt(int i);

private:
	std::vector<SpriteAnimator> animators;
};
}
