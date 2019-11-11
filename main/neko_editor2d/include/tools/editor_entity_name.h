#pragma once

#include <engine/component.h>

namespace neko::editor
{

class EntityNameManager : public ComponentManager<std::string, ComponentType(NekoComponentType::EMPTY)>
{
public:
    explicit EntityNameManager(EntityManager& entityManager);
    Index AddComponent(EntityManager& entityManager, Entity entity) override;

    void SetComponent(Entity entity, const std::string& component) override;

    void ParseComponentJson(const json& componentJson, Entity entity) override;

    json SerializeComponentJson(Entity entity) override;

private:
    EntityManager& entityManager_;

};

}