/*
 MIT License

 Copyright (c) 2022 SAE Institute Switzerland AG

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

#include "b2/body_manager.h"
#include "b2/physics_manager.h"

namespace neko
{
BodyManager::BodyManager(EntityManager& entityManager, Physics2dManager& physicsManager):
    ComponentManager(entityManager),
    physicsManager_(physicsManager)
{
}

void BodyManager::AddBody(Entity entity, const phys2d::pb::Body2dDef& pbBodyDef)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(pbBodyDef.position_x(), pbBodyDef.position_y());
    bodyDef.linearVelocity.Set(pbBodyDef.linear_velocity_x(), pbBodyDef.linear_velocity_y());
    bodyDef.angle = pbBodyDef.angle();
    bodyDef.angularVelocity = pbBodyDef.angular_velocity();
    bodyDef.linearDamping = pbBodyDef.linear_damping();
    bodyDef.angularDamping = pbBodyDef.angular_damping();
    bodyDef.allowSleep = pbBodyDef.allow_sleep();
    bodyDef.awake = pbBodyDef.awake();
    bodyDef.fixedRotation = pbBodyDef.fixed_rotation();
    bodyDef.bullet = pbBodyDef.bullet();
    switch (pbBodyDef.type())
    {
    case phys2d::pb::Body2dDef_BodyType_STATIC: 
        bodyDef.type = b2_staticBody;
        break;
    case phys2d::pb::Body2dDef_BodyType_DYNAMIC: 
        bodyDef.type = b2_dynamicBody;
        break;
    case phys2d::pb::Body2dDef_BodyType_KINEMATIC: 
        bodyDef.type = b2_kinematicBody;
        break;
   default: ;
    }
    bodyDef.enabled = pbBodyDef.enable();
    bodyDef.gravityScale = pbBodyDef.gravity_scale();

    AddComponent(entity);
    auto& [impl] = components_[entity];
    impl = physicsManager_.CreateBody(bodyDef);
}

void BodyManager::DestroyComponent(Entity entity)
{
    auto& body = components_[entity];
    physicsManager_.DestroyBody(body);
    body.impl = nullptr;
    ComponentManager::DestroyComponent(entity);
}

phys2d::pb::Body2dDef GenerateDefaultBodyDef()
{
    phys2d::pb::Body2dDef bodyDef;
    bodyDef.set_position_x(0.0f);
    bodyDef.set_position_y(0.0f);
    bodyDef.set_linear_velocity_x(0.0f);
    bodyDef.set_linear_velocity_y(0.0f);
    bodyDef.set_linear_damping(0.0f);
    bodyDef.set_angle(0.0f);
    bodyDef.set_angular_velocity(0.0f);
    bodyDef.set_angular_damping(0.0f);
    bodyDef.set_allow_sleep(true);
    bodyDef.set_awake(true);
    bodyDef.set_fixed_rotation(false);
    bodyDef.set_bullet(false);
    bodyDef.set_type(phys2d::pb::Body2dDef_BodyType_STATIC);
    bodyDef.set_enable(true);
    bodyDef.set_gravity_scale(1.0f);
    return bodyDef;
}
}
