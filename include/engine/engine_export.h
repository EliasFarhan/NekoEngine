#pragma once

namespace neko
{
class EntityManager;
class PrefabManager;
class SceneManager;
struct EngineExport
{
    EntityManager& entityManager;
    PrefabManager& prefabManager;
    SceneManager& sceneManager;
};
}