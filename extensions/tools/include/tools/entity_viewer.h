#pragma once

#include <string>
#include <vector>

#include <engine/entity.h>
namespace editor
{
class EntityViewer
{
public:
    void Update(neko::EntityManager& entityManager);

    neko::Entity GetSelectedEntity(){return selectedEntity_;};
private:
    std::vector<neko::Entity> entities_;
    std::vector<std::string> entitiesName_;

    neko::Entity selectedEntity_ = neko::INVALID_ENTITY;
};
}