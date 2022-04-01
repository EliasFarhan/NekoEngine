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

#include "b2/physics_manager.h"

namespace neko
{
Physics2dManager::Physics2dManager()
{
    config_.set_fixed_timestep(defaultFixedStep);
    config_.set_gravity_x(defaultGravity.x);
    config_.set_gravity_y(defaultGravity.y);
    config_.set_velocity_iterations(defaultVelocityIterations);
    config_.set_position_iterations(defaultPositionIterations);
    
}

Physics2dManager::Physics2dManager(phys2d::pb::PhysicsConfig&& config) : config_(std::move(config))
{
}

void Physics2dManager::Init()
{
    world_ = std::make_unique<b2World>(b2Vec2{config_.gravity_x(), config_.gravity_y()});

}

void Physics2dManager::Update(seconds dt)
{
    currentTime_ += dt.count();
    const auto fixedTimeStep = config_.fixed_timestep();
    const auto velocityIterations = config_.velocity_iterations();
    const auto positionIterations = config_.position_iterations();
    while(currentTime_ > fixedTimeStep)
    {
        world_->Step(fixedTimeStep, velocityIterations, positionIterations);
        currentTime_ -= fixedTimeStep;
    }
}

void Physics2dManager::Destroy()
{
    world_.reset();
}

b2Body* Physics2dManager::CreateBody(const b2BodyDef& bodyDef) const
{
    return world_->CreateBody(&bodyDef);
}

void Physics2dManager::DestroyBody(const Body& body) const
{
    world_->DestroyBody(body.impl);
}
}
