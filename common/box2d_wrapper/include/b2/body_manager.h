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

#include "engine/component.h"
#include "box2d/b2_body.h"
#include "proto/phys2d.pb.h"

namespace neko
{
class Physics2dManager;

struct Body
{
    b2Body* impl = nullptr;
};
class BodyManager final : ComponentManager<Body, static_cast<EntityMask>(ComponentType::BODY2D)>
{
public:
    BodyManager(EntityManager& entityManager, Physics2dManager& physicsManager);
    void AddBody(Entity entity, const phys2d::pb::Body2dDef& bodyDef);
    void DestroyComponent(Entity entity) override;
private:
    Physics2dManager& physicsManager_;
};

phys2d::pb::Body2dDef GenerateDefaultBodyDef();

}
