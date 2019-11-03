#pragma once




namespace neko
{
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

struct NekoEditorExport
{
    EntityManager& entityManager;
    Position2dManager& position2dManager;
    Scale2dManager& scale2dManager;
    Rotation2dManager& rotation2dManager;
    sfml::Transform2dManager& transform2dManager;
    EditorSceneManager& sceneManager;
    box2d::BodyDef2dManager& bodyDef2dManager;
    sfml::SpriteManager& spriteManager;
    sfml::TextureManager& textureManager;
    sfml::SpineManager& spineManager;
    BoxColliderDefManager& boxColliderDefManager_;
    CircleColliderDefManager& circleColliderDefManager_;
    PolygonColldierDefManager& polygonColldierDefManager_;
    ColliderDefManager& coll;
    EditorPrefabManager& prefabManager;
    NekoEditor& editor;
};
}