#pragma once

#include "engine/scene.h"
#include "sfml_engine/engine_export.h"

namespace neko
{
enum class NekoComponentType : ComponentType;
}
namespace neko::sfml
{
class SfmlBasicSceneManager : public SceneManager
{
public:
    explicit SfmlBasicSceneManager(SfmlBasicEngineExport& engineExport);
    void ParseComponentJson(json& componentJson, Entity entity) override;
    void ParseEntityJson(json& entityJson) override;

protected:
    Position2dManager& position2dManager_;
    Rotation2dManager& rotation2dManager_;
    Scale2dManager& scale2dManager_;

    Transform2dManager& transformManager_;
    SpriteManager& spriteManager_;
    SpineManager& spineManager_;
	SpineBoneFollowerManager& spineBoneFollowerManager_;
};
class SfmlFullSceneManager : public SfmlBasicSceneManager
{
public:
    explicit SfmlFullSceneManager(SfmlFullEngineExport& engineExport);
protected:
    box2d::Body2dManager& bodyManager_;
};
}