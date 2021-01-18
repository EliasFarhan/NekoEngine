#pragma once

#include "engine/entity.h"
#include "engine/component.h"

namespace neko::editor
{
struct EntityData
{
    std::string name;
    Entity nextSibling = INVALID_ENTITY;
    Entity firstChild = INVALID_ENTITY;
};

class EntityDataManager : public ComponentManager<EntityData, EntityMask(ComponentType::EMPTY)>
{
public:
    using ComponentManager<EntityData, EntityMask(ComponentType::EMPTY)>::ComponentManager;
};
}