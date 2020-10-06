/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

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
#include "asteroid/physics_manager.h"

namespace neko::asteroid
{

PhysicsManager::PhysicsManager(EntityManager& entityManager) :
    bodyManager_(entityManager), boxManager_(entityManager), entityManager_(entityManager)
{

}

void PhysicsManager::FixedUpdate(seconds dt)
{
    for(Entity entity = 0; entity < entityManager_.get().GetEntitiesSize(); entity++)
    {
        if (!entityManager_.get().HasComponent(entity, EntityMask(neko::ComponentType::BODY2D)))
            continue;
        auto body = bodyManager_.GetComponent(entity);
        body.position += body.velocity * dt.count();
        body.rotation += body.angularVelocity * dt.count();
        bodyManager_.SetComponent(entity, body);
    }
}

void PhysicsManager::SetBody(Entity entity, const Body& body)
{
    bodyManager_.SetComponent(entity, body);
}

const Body& PhysicsManager::GetBody(Entity entity) const
{
    return bodyManager_.GetComponent(entity);
}

void PhysicsManager::AddBody(Entity entity)
{
    bodyManager_.AddComponent(entity);
}

}