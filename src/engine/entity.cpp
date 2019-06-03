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
#include "engine/globals.h"

namespace neko
{

EntityManager::EntityManager()
{
    m_MaskArray.reserve(InitEntityNmb);
}

EntityMask EntityManager::GetMask(Entity entity)
{
    return m_MaskArray[entity - 1];
}

Entity EntityManager::CreateEntity(Entity wantedEntity)
{
    for (Entity entity = 1U; entity <= m_MaskArray.size(); entity++)
    {
        if (m_MaskArray[entity - 1] == INVALID_ENTITY)
        {
            return entity;
        }
    }
    return INVALID_ENTITY;
}

void EntityManager::DestroyEntity(Entity entity)
{
    m_MaskArray[entity - 1] = INVALID_ENTITY;
}

bool EntityManager::HasComponent(Entity entity, ComponentType componentType)
{
    return (m_MaskArray[entity - 1] & static_cast<EntityMask>(componentType)) == static_cast<EntityMask>(componentType);
}

void EntityManager::AddComponentType(Entity entity, ComponentType componentType)
{
    m_MaskArray[entity - 1] = m_MaskArray[entity - 1] | static_cast<EntityMask>(componentType);
}

void EntityManager::RemoveComponentType(Entity entity, ComponentType componentType)
{
    m_MaskArray[entity - 1] &= ~static_cast<EntityMask>(componentType);
}
}
