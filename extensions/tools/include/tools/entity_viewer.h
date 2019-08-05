#pragma once

#include <string>
#include <vector>

#include <engine/entity.h>
#include <engine/transform.h>
#include "editor_scene.h"

namespace editor
{
class EntityViewer
{
public:
    void Update(neko::EntityManager& entityManager, EditorSceneManager& sceneManager,
                neko::Transform2dManager& transformManager);

    void Reset()
    { selectedEntity_ = neko::INVALID_ENTITY; }

    neko::Entity GetSelectedEntity()
    { return selectedEntity_; }

private:
    void DrawEntityHierarchy(neko::Entity entity,
                             size_t index,
                             neko::Transform2dManager& transformManager,
                             std::set<neko::Entity>& entitySet,
                             bool draw);

    std::vector<neko::Entity> entities_;
    std::vector<std::string> entitiesName_;

    neko::Entity selectedEntity_ = neko::INVALID_ENTITY;
};
}