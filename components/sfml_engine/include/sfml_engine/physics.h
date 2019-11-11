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
#include <vector>
#include <mathematics/vector.h>
#include <SFML/Audio.hpp>
#include "collider.h"
#include "engine/component.h"

namespace neko
{
float pixel2unit(float f);
Vec2f pixel2unit(sf::Vector2f v);

float unit2pixel(float f);
sf::Vector2f unit2pixel(Vec2f v);

struct Configuration;
namespace box2d
{

/**
 * \brief system using box2d to manage physics
 */
class Physics2dManager : public System
{
public:
	Physics2dManager(ContactListener& contactListener);
	void SetConfiguration(Configuration* config);
    void Init() override;

    void Update(float dt) override;

    void Destroy() override;


    b2Body* CreateBody(b2BodyDef& bodyDef, b2FixtureDef* fixturesDef, size_t fixturesNmb);
    void DestroyBody(b2Body* body) const;


    /**
     * \brief value used to define how many pixels per meter to translate from graphics to physics
     */
    static float pixelPerMeter;
    static float pixelPerUnit;
    void AddCollider(const Collider& collider);

    b2Body* GetBodyAt(Index i);

    std::vector<Collider>& GetColliderVector()
    { return colliders_; }

private:
    friend class Transform2dManager;

    std::unique_ptr<b2World> world_ = nullptr;
    std::vector<b2Body*> bodies_;
    std::vector<Collider> colliders_;
    Configuration* config_ = nullptr;
    ContactListener& collisionListener_;
};


class Body2dManager : public ComponentManager<b2Body*, ComponentType(NekoComponentType::BODY2D)>
{
public:
	explicit Body2dManager(Physics2dManager& physics2dManager);

	void DestroyComponent(EntityManager& entityManager, Entity entity) override;
private:
	Physics2dManager& physics2dManager_;
};

class BodyDef2dManager : public ComponentManager<b2BodyDef, ComponentType(NekoComponentType::BODY2D)>
{
public:
    void ParseComponentJson(const json& componentJson, Entity entity) override;

    json SerializeComponentJson(Entity entity) override;

public:

};

class Collider2dManager : public ComponentManager<Collider, ComponentType(NekoComponentType::BOX_COLLIDER2D)>
{
};

float pixel2meter(float f);

b2Vec2 pixel2meter(sf::Vector2f v);


float meter2pixel(float f);

sf::Vector2f meter2pixel(b2Vec2 v);





}
}
