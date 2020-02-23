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

#include "globals.h"
#include <engine/entity.h>
#include <mathematics/matrix.h>
#include "engine/component.h"
#include "mathematics/vector.h"
#include "mathematics/quaternion.h"

namespace neko
{


class Position2dManager : public ComponentManager<Vec2f, ComponentType::POSITION2D>
{
};

class Scale2dManager : public ComponentManager<Vec2f, ComponentType::SCALE2D>
{
public:
    using ComponentManager::ComponentManager;
    Index AddComponent(Entity entity) override;

};

class Rotation2dManager : public ComponentManager<float, ComponentType::ROTATION2D>
{

};


class Position3dManager : public ComponentManager<Vec3f, ComponentType::POSITION3D>
{

};


class Rotation3dManager : public ComponentManager<Quaternion, ComponentType::ROTATION3D>
{

};

class Scale3dManager : public ComponentManager<Vec3f, ComponentType::SCALE3D>
{
public:
    using ComponentManager::ComponentManager;
    Index AddComponent(Entity entity) override;

};

class Transform2dManager : public ComponentManager<Mat3f, ComponentType::TRANSFORM2D>
{
public:
protected:

};

class Transform3dManager : public ComponentManager<Mat4f, ComponentType::TRANSFORM3D>
{
public:
	using ComponentManager::ComponentManager;
	void UpdateTransform(Entity entity);
protected:
    Position3dManager position3DManager_;
    Scale3dManager scale3DManager_;
    Rotation3dManager rotation3DManager_;
};




}
