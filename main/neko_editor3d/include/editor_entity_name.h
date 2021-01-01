#pragma once

#include "engine/entity.h"
#include "engine/component.h"

namespace neko::editor
{
class EntityNameManager : public ComponentManager<std::string, EntityMask(ComponentType::EMPTY)>
{
public:
    using ComponentManager<std::string, EntityMask(ComponentType::EMPTY)>::ComponentManager;
};
}