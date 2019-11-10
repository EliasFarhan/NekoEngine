#pragma once

#include <engine/engine_export.h>

namespace neko
{
class Position2dManager;
class Scale2dManager;
class Rotation2dManager;
class EntityManager;

namespace sfml
{
class Transform2dManager;

class SpriteManager;
class SpineManager;
class SpineBoneFollowerManager;
}
namespace box2d
{
class Physics2dManager;
class Body2dManager;
class ContactListener;
}
}
namespace neko::sfml
{
struct SfmlBasicEngineExport : EngineExport
{
    Position2dManager& position2dManager;
    Scale2dManager& scale2dManager;
    Rotation2dManager& rotation2dManager;
    Transform2dManager& transform2dManager;

    SpriteManager& spriteManager;
    SpineManager& spineManager;
	SpineBoneFollowerManager& spineBoneFollowerManager;
};
struct SfmlFullEngineExport : SfmlBasicEngineExport
{
    box2d::Physics2dManager& physics2dManager;
    box2d::Body2dManager& body2dManager;
};
}