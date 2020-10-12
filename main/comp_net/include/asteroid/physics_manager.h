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

#pragma once
#include "mathematics/vector.h"
#include "engine/component.h"

namespace neko::asteroid
{

enum class BodyType
{
    DYNAMIC,
    STATIC
};
struct Body
{
    Vec2f position = Vec2f::zero;
    Vec2f velocity = Vec2f::zero;
    degree_t angularVelocity = degree_t(0.0f);
    degree_t rotation = degree_t(0.0f);
    BodyType bodyType = BodyType::DYNAMIC;
};

struct Box
{
    Vec2f extends = Vec2f::one;
    bool isTrigger = false;
};

class OnCollisionInterface
{
public:
    virtual void OnCollision(Entity entity1, Entity entity2) = 0;
};

class BodyManager : public ComponentManager<Body, EntityMask(neko::ComponentType::BODY2D)>
{
    using ComponentManager::ComponentManager;
};
class BoxManager : public ComponentManager<Box, EntityMask(neko::ComponentType::BOX_COLLIDER2D)>
{
    using ComponentManager::ComponentManager;
};

class PhysicsManager
{
public:
    explicit PhysicsManager(EntityManager& entityManager);
    PhysicsManager (const PhysicsManager& physicsManager) = default;
    void FixedUpdate(seconds dt);
    [[nodiscard]] const Body& GetBody(Entity entity) const;
    void SetBody(Entity entity, const Body& body);
    void AddBody(Entity entity);

    void AddBox(Entity entity);
    void SetBox(Entity entity, const Box& box);
    [[nodiscard]] const Box& GetBox(Entity entity) const;

    void RegisterCollisionListener(OnCollisionInterface& collisionInterface);
private:
    std::reference_wrapper<EntityManager> entityManager_;
    BodyManager bodyManager_;
    BoxManager boxManager_;
    Action<Entity, Entity> onCollisionAction_;
};

}