#pragma once

#include "entity.h"

/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <engine/globals.h>
#include <utilities/vector_utility.h>
#include <utilities/json_utility.h>
#include <set>


namespace neko
{

enum class NekoComponentType : ComponentType
{
    POSITION2D = 1 << 1,
    SCALE2D = 1 << 2,
    ROTATION2D = 1 << 3,
    TRANSFORM2D = ROTATION2D|SCALE2D|POSITION2D,
    POSITION3D = 1 << 4,
    SCALE3D = 1 << 5,
    ANGLE3D = 1 << 6,
    SPRITE2D = 1 << 7,
    SPINE_ANIMATION = 1 << 8,
    BODY2D = 1 << 9,
    BOXCOLLIDER2D = 1 << 10,
    CIRCLECOLLIDER2D = 1 << 11,
    POLYGONCOLLIDER2D = 1 << 12,
    CONVEX_SHAPE2D = 1 << 13,
    EMPTY = 1 << 14,

};

const std::set<NekoComponentType>& GetComponentTypeSet();

struct Component
{
    Entity entity = INVALID_ENTITY;
    ComponentType componentType = INVALID_COMPONENT_TYPE;
};

template<typename T, ComponentType componentType>
class ComponentManager
{
public:
    ComponentManager()
    {
        ResizeIfNecessary(components_, INIT_ENTITY_NMB - 1, T{});
    }

    explicit ComponentManager(T default_value)
    {
        ResizeIfNecessary(components_, INIT_ENTITY_NMB - 1, default_value);
    }

    virtual ~ComponentManager()
    {};

    virtual Index AddComponent(EntityManager& entityManager, Entity entity);

    virtual void DestroyComponent(EntityManager& entityManager, Entity entity);

    virtual void SetComponent(Entity entity, T component)
    { components_[entity] = component; }

    const T& GetConstComponent(Entity entity) const
    { return components_[entity]; }

    T& GetComponent(Entity entity)
    { return components_[entity]; }

    const T* GetConstComponentPtr(Entity entity) const
    { return &components_[entity]; }

    T* GetComponentPtr(Entity entity)
    { return &components_[entity]; }

    const std::vector<T>& GetConstComponentsVector() const
    { return components_; }

    std::vector<T>& GetComponentsVector()
    { return components_; }

    virtual void ParseComponentJson(json& componentJson, Entity entity)
    {};

    virtual json SerializeComponentJson(Entity entity)
    { return json(); };
protected:
    std::vector<T> components_;
};

template<typename T, ComponentType componentType>
Index ComponentManager<T, componentType>::AddComponent(EntityManager& entityManager, Entity entity)
{
    ResizeIfNecessary(components_, entity, T{});
    entityManager.AddComponentType(entity, componentType);
    return entity;
}

template<typename T, ComponentType componentType>
void ComponentManager<T, componentType>::DestroyComponent(EntityManager& entityManager, Entity entity)
{
    entityManager.RemoveComponentType(entity, componentType);
}

}
