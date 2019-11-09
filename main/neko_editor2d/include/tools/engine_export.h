#pragma once

#include <engine/engine_export.h>
#include <sfml_engine/engine_export.h>

namespace neko
{
    struct Configuration;
    class EntityManager;
    class Position2dManager;
    class Scale2dManager;
    class Rotation2dManager;
    namespace sfml
    {
    class Transform2dManager;
    class SpriteManager;
    class SpineManager;
    class TextureManager;
    }
    namespace box2d
    {
    class BodyDef2dManager;
    }
}

namespace neko::editor
{
class EditorSceneManager;
class EditorPrefabManager;
class NekoEditor;
class BoxColliderDefManager;
class CircleColliderDefManager;
class PolygonColldierDefManager;
class ColliderDefManager;
class EntityViewer;
class Inspector;
class SceneViewer;
class EntityNameManager;
class NekoEditor;
struct NekoEditorExport : sfml::SfmlBasicEngineExport
{
    EditorSceneManager& sceneManager;
    box2d::BodyDef2dManager& bodyDef2dManager;
    sfml::TextureManager& textureManager;
    BoxColliderDefManager& boxColliderDefManager;
    CircleColliderDefManager& circleColliderDefManager;
    PolygonColldierDefManager& polygonColldierDefManager;
    ColliderDefManager& colliderDefManager;
    EditorPrefabManager& prefabManager;
    Configuration& config;
    NekoEditor& editor;
    EntityNameManager& entityNameManager;
};
}