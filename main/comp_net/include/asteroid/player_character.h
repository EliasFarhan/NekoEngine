#pragma once

#include <engine/transform.h>
#include "packet_type.h"
#include "engine/system.h"
#include "engine/component.h"
#include "gl/texture.h"
#include "gl/shader.h"
#include "gl/shape.h"

namespace neko::asteroid
{


struct PlayerCharacter
{
    Vec2f velocity = Vec2f::zero;
    Vec2f position = Vec2f::zero;
    degree_t rotation = degree_t(0.0f);
};

class PlayerCharacterManager : public ComponentManager<PlayerCharacter, neko::ComponentType(ComponentType::PLAYER_CHARACTER)>
{
 using ComponentManager::ComponentManager;
};

} // namespace neko::asteroid
