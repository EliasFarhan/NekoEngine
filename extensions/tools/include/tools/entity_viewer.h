#pragma once

#include <string>
#include <vector>

#include <engine/entity.h>
#include "editor_scene.h"

namespace editor
{
class EntityViewer
{
public:
    void Update(neko::EntityManager& entityManager, EditorSceneManager& sceneManager);
    void Reset(){selectedEntity_ = neko::INVALID_ENTITY; }
    neko::Entity GetSelectedEntity(){return selectedEntity_; }
private:
    std::vector<neko::Entity> entities_;
    std::vector<std::string> entitiesName_;

    neko::Entity selectedEntity_ = neko::INVALID_ENTITY;
};
}