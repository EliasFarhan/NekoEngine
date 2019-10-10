#pragma once


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

#include <engine/entity.h>
#include <engine/globals.h>
#include <utilities/vector_utility.h>
#include <utilities/json_utility.h>
#include <set>


namespace neko
{

enum class NekoComponentType : ComponentType
{
    EMPTY = 1u << 0u,
    POSITION2D = 1u << 1u,
    SCALE2D = 1u << 2u,
    ROTATION2D = 1u << 3u,
    TRANSFORM2D = ROTATION2D|SCALE2D|POSITION2D,
    POSITION3D = 1u << 4u,
    SCALE3D = 1u << 5u,
    ANGLE3D = 1u << 6u,
    SPRITE2D = 1u << 7u,
    SPINE_ANIMATION = 1u << 8u,
    BODY2D = 1u << 9u,
    BOX_COLLIDER2D = 1u << 10u,
    CIRCLE_COLLIDER2D = 1u << 11u,
    POLYGON_COLLIDER2D = 1u << 12u,
    CONVEX_SHAPE2D = 1u << 13u,
    SPINE_FOLLOW_BONE = 1u << 14u,
    PREFAB = 1u << 15u,


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

    virtual void ParseComponentJson([[maybe_unused]]json& componentJson, [[maybe_unused]]Entity entity)
    {};

    virtual json SerializeComponentJson([[maybe_unused]]Entity entity)
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
