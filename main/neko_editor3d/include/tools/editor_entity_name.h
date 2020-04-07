#pragma once

#include <engine/component.h>

namespace neko::editor
{

class EntityNameManager : public ComponentManager<std::string, ComponentType::EMPTY>
{
public:
    explicit EntityNameManager(EntityManager& entityManager);
    Index AddComponent(Entity entity) override;

    void SetComponent(Entity entity, const std::string& component) override;
private:
    EntityManager& entityManager_;

};

}