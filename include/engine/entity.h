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

#include <vector>

namespace neko
{
using Entity = unsigned;
using EntityMask = unsigned;
enum class ComponentType : unsigned;
const Entity INVALID_ENTITY = 0U;

class EntityManager
{
public:
    EntityManager();

    EntityMask GetMask(Entity entity);

    Entity CreateEntity(Entity wantedEntity = 0u);

    void DestroyEntity(Entity entity);

    bool HasComponent(Entity entity, ComponentType componentType);

    void AddComponentType(Entity entity, ComponentType componentType);

    void RemoveComponentType(Entity entity, ComponentType componentType);

    std::vector<EntityMask> m_MaskArray;
};
}
