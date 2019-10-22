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

#include "Box2D/Dynamics/b2Fixture.h"
#include "engine/entity.h"
#include "Box2D/Dynamics/b2WorldCallbacks.h"

namespace neko::box2d
{

class MainEngine;

/**
 * \brief class used in the void* of box2d fixture to be able to get info from contact and collisions
 */
struct Collider
{
    b2Fixture* fixture;
    b2Body* body;
    neko::Entity entity;
};

/**
 * \brief override the default box2d contact listener and communicate directly to the MainEngine
 */
class ContactListener : public b2ContactListener
{
public:
    ~ContactListener() = default;

    void BeginContact(b2Contact* contact) override;

    void EndContact(b2Contact* contact) override;
};
}
