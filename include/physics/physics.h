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
#include "engine/system.h"
#include <Box2D/Box2D.h>
#include "SFML/System/Vector2.hpp"
#include <vector>
#include "collider.h"

namespace neko
{
struct Configuration;

/**
 * \brief system using box2d to manage physics
 */
class Physics2dManager : public System
{
public:
    void Init() override;

    void Update() override;

    void Destroy() override;


    b2Body* CreateBody(b2BodyDef& bodyDef, b2FixtureDef* fixturesDef, size_t fixturesNmb);

    b2World* world = nullptr;
    std::vector<b2Body*> bodies;
    std::vector<Collider> colliders;
    Configuration* config = nullptr;
    CollisionListener collisionListener;
    /**
     * \brief value used to define how many pixels per meter to translate from graphics to physics
     */
    static float pixelPerMeter;
};

float meter2pixel(float v);

float pixel2meter(float v);

sf::Vector2f meter2pixel(const b2Vec2& v);

b2Vec2 pixel2meter(const sf::Vector2f& v);

}
