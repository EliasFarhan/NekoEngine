/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include <sfml_engine/collider.h>
#include <sfml_engine/engine.h>
#include <Box2D/Box2D.h>
#include <engine/engine.h>

namespace neko::box2d
{


void ContactListener::BeginContact(b2Contact* contact)
{
	const auto* colliderA = static_cast<Collider*>(contact->GetFixtureA()->GetUserData());
	const auto* colliderB = static_cast<Collider*>(contact->GetFixtureB()->GetUserData());
	BasicEngine::GetInstance<sfml::SfmlFullEngine>()->OnBeginContact(colliderA, colliderB);
}

void ContactListener::EndContact(b2Contact* contact)
{
	const auto* colliderA = static_cast<Collider*>(contact->GetFixtureA()->GetUserData());
	const auto* colliderB = static_cast<Collider*>(contact->GetFixtureB()->GetUserData());
	BasicEngine::GetInstance<sfml::SfmlFullEngine>()->OnEndContact(colliderA, colliderB);
}
}