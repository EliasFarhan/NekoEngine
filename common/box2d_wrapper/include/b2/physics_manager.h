#pragma once
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


#include "body_manager.h"
#include "box2d/b2_world.h"
#include "engine/system.h"
#include "mathematics/vec2.h"
#include "proto/phys2d.pb.h"

namespace neko
{


class Physics2dManager : public SystemInterface
{
public:
    Physics2dManager();
    Physics2dManager(phys2d::pb::PhysicsConfig&& config);

    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;
    b2Body* CreateBody(const b2BodyDef& bodyDef) const;
    void DestroyBody(const Body& body) const;
private:
    std::unique_ptr<b2World> world_;
    
    float currentTime_ = 0.0f;
    phys2d::pb::PhysicsConfig config_;
    static constexpr Vec2f defaultGravity{ 0.0f, 9.81f };
    static constexpr float defaultFixedStep = 0.02f;
    static constexpr std::int32_t defaultVelocityIterations = 6;
    static constexpr std::int32_t defaultPositionIterations = 6;
};
}
