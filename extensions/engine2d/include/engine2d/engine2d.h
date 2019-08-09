#pragma once

#include <engine/engine.h>
#include <engine/transform.h>
#include <graphics/sprite.h>
#include <graphics/spine.h>

namespace neko
{

class Engine2d : public BasicEngine
{
public:
    void Update(float dt) override;

public:
    using BasicEngine::BasicEngine;
protected:
    Transform2dManager transformManager_;
    Physics2dManager physicsManager_;
    SpriteManager spriteManager_;
    SpineManager spineManager_;
};

}