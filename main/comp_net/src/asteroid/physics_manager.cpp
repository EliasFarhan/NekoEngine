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
#include "asteroid/game.h"

namespace neko::asteroid
{

PhysicsManager::PhysicsManager(EntityManager& entityManager) :
    bodyManager_(entityManager), boxManager_(entityManager), entityManager_(entityManager)
{

}

bool Box2Box(float r1x, float r1y, float r1w, float r1h, float r2x, float r2y, float r2w, float r2h)
{
    return r1x + r1w >= r2x &&    // r1 right edge past r2 left
        r1x <= r2x + r2w &&    // r1 left edge past r2 right
        r1y + r1h >= r2y &&    // r1 top edge past r2 bottom
        r1y <= r2y + r2h;
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
    for (Entity entity = 0; entity < entityManager_.get().GetEntitiesSize(); entity++)
    {
        if(!entityManager_.get().HasComponent(entity, 
            EntityMask(neko::ComponentType::BODY2D)|EntityMask(neko::ComponentType::BOX_COLLIDER2D)) ||
            entityManager_.get().HasComponent(entity, EntityMask(neko::asteroid::ComponentType::DESTROYED)))
            continue;
        for (Entity otherEntity = entity; otherEntity < entityManager_.get().GetEntitiesSize(); otherEntity++)
        {
            if(entity == otherEntity)
                continue;
            if (!entityManager_.get().HasComponent(otherEntity,
                EntityMask(neko::ComponentType::BODY2D) | EntityMask(neko::ComponentType::BOX_COLLIDER2D)) ||
                entityManager_.get().HasComponent(entity, EntityMask(neko::asteroid::ComponentType::DESTROYED)))
                continue;
            const Body& body1 = bodyManager_.GetComponent(entity);
            const Box& box1 = boxManager_.GetComponent(entity);

            const Body& body2 = bodyManager_.GetComponent(otherEntity);
            const Box& box2 = boxManager_.GetComponent(otherEntity);

            if(Box2Box(
                body1.position.x - box1.extends.x, 
                body1.position.y - box1.extends.y,
                box1.extends.x * 2.0f,
                box1.extends.y * 2.0f, 
                body2.position.x - box2.extends.x,
                body2.position.y - box2.extends.y,
                box2.extends.x * 2.0f,
                box2.extends.y * 2.0f))
            {
                onCollisionAction_.Execute(entity, otherEntity);
            }

        }
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

void PhysicsManager::AddBox(Entity entity)
{
    boxManager_.AddComponent(entity);
}

void PhysicsManager::SetBox(Entity entity, const Box& box)
{
    boxManager_.SetComponent(entity, box);
}

const Box& PhysicsManager::GetBox(Entity entity) const
{
    return boxManager_.GetComponent(entity);
}

void PhysicsManager::RegisterCollisionListener(OnCollisionInterface& collisionInterface)
{
    onCollisionAction_.RegisterCallback(
        [&collisionInterface](Entity entity1, Entity entity2) { collisionInterface.OnCollision(entity1, entity2); });
}
}
