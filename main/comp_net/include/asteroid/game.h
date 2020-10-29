/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

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

#pragma once
#include "mathematics/angle.h"
#include "engine/entity.h"
#include "engine/component.h"
#include "graphics/color.h"

namespace neko::asteroid
{

const std::uint32_t maxPlayerNmb = 2;
const short playerHealth = 5;
const float playerSpeed = 1.0f;
const degree_t playerAngularSpeed = degree_t(90.0f);
const float playerShootingPeriod = 0.3f;
const float bulletSpeed = 2.0f;
const float bulletScale = 0.2f;
const float bulletPeriod = 3.0f;
const float playerInvincibilityPeriod = 1.5f;
const float invincibilityFlashPeriod = 0.5f;

const std::array<Color4, std::max(maxPlayerNmb, 4u)> playerColors =
{
  {
        Color4(Color::red,1.0f),
        Color4(Color::blue,1.0f),
        Color4(Color::yellow, 1.0f),
        Color4(Color::cyan, 1.0f)
    }
};

const std::array<Vec2f, std::max(4u, maxPlayerNmb)> spawnPositions
{
        Vec2f(0,1),
        Vec2f(0,-1),
        Vec2f(1,0),
        Vec2f(-1,0),
};

const std::array<degree_t, std::max(4u, maxPlayerNmb)> spawnRotations{
    degree_t(0.0f),
    degree_t(180.0f),
    degree_t(-90.0f),
    degree_t(90.0f)
};

enum class ComponentType : EntityMask
{
    PLAYER_CHARACTER = static_cast<EntityMask>(neko::ComponentType::OTHER_TYPE),
    BULLET = static_cast<EntityMask>(neko::ComponentType::OTHER_TYPE) << 1u,
    ASTEROID = static_cast<EntityMask>(neko::ComponentType::OTHER_TYPE) << 2u,
    PLAYER_INPUT = static_cast<EntityMask>(neko::ComponentType::OTHER_TYPE) << 3u,
    DESTROYED = static_cast<EntityMask>(neko::ComponentType::OTHER_TYPE) << 4u,
};

namespace PlayerInput
{
enum PlayerInput : std::uint8_t
{
    NONE = 0u,
    UP = 1u << 0u,
    DOWN = 1u << 1u,
    LEFT = 1u << 2u,
    RIGHT = 1u << 3u,
    SHOOT = 1u << 4u,
};
}
}
