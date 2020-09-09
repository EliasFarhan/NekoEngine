#pragma once

namespace neko::asteroid
{

const std::uint32_t maxPlayerNmb = 2;
const float playerSpeed = 1.0f;
const degree_t playerAngularSpeed = degree_t(90.0f);

enum class ComponentType : EntityMask
{
    PLAYER_CHARACTER = static_cast<EntityMask>(neko::ComponentType::OTHER_TYPE),
    BULLET = static_cast<EntityMask>(neko::ComponentType::OTHER_TYPE) << 1u,
    ASTEROID = static_cast<EntityMask>(neko::ComponentType::OTHER_TYPE) << 2u,
    PLAYER_INPUT = static_cast<EntityMask>(neko::ComponentType::OTHER_TYPE) << 3u,

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
