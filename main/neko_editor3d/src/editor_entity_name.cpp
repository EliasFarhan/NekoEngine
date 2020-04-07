#include <sstream>
#include "tools/editor_entity_name.h"

namespace neko::editor
{


Index EntityNameManager::AddComponent(Entity entity)
{
    std::ostringstream oss;
    oss << "Entity " << entity;
    const auto index = ComponentManager::AddComponent(entity);
    SetComponent(entity, oss.str());
    return index;
}

void EntityNameManager::SetComponent(Entity entity, const std::string& component)
{
    entityManager_.SetEntityName(entity, component);
    ComponentManager::SetComponent(entity, component);
}

EntityNameManager::EntityNameManager(EntityManager& entityManager) : ComponentManager(entityManager), entityManager_(entityManager)
{

}
}