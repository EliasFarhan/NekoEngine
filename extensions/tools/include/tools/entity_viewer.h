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
    EntityViewer(NekoEditor& editor);
    void Update();

    void Reset()
    { selectedEntity_ = neko::INVALID_ENTITY; }

    neko::Entity GetSelectedEntity()
    { return selectedEntity_; }

private:
    void DrawEntityHierarchy(neko::Entity entity, size_t index, std::set<neko::Entity>& entitySet, bool draw,
                             bool destroy);

    std::vector<neko::Entity> entities_;
    std::vector<std::string> entitiesName_;
    NekoEditor& nekoEditor_;
    neko::Entity selectedEntity_ = neko::INVALID_ENTITY;
};
}