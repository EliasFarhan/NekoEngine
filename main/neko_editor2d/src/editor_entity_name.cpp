#include <sstream>
#include "tools/editor_entity_name.h"

namespace neko::editor
{


Index EntityNameManager::AddComponent(EntityManager& entityManager, Entity entity)
{
    std::ostringstream oss;
    oss << "Entity " << entity;
    const auto index = ComponentManager::AddComponent(entityManager, entity);
    SetComponent(entity, oss.str());
    return index;
}

void EntityNameManager::SetComponent(Entity entity, const std::string& component)
{
    entityManager_.SetEntityName(entity, component);
    ComponentManager::SetComponent(entity, component);
}

void EntityNameManager::ParseComponentJson(const json& componentJson, Entity entity)
{
    ComponentManager::ParseComponentJson(componentJson, entity);
}

json EntityNameManager::SerializeComponentJson(Entity entity)
{
    return ComponentManager::SerializeComponentJson(entity);
}

EntityNameManager::EntityNameManager(EntityManager& entityManager) : entityManager_(entityManager)
{

}
}