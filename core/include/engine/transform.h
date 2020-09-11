#pragma once
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

#include "globals.h"
#include <engine/entity.h>
#include <mathematics/matrix.h>
#include "engine/component.h"
#include "graphics/graphics.h"
#include "mathematics/vector.h"
#include "mathematics/quaternion.h"

namespace neko
{


class Position2dManager : public ComponentManager<Vec2f, ComponentType::POSITION2D>
{
    using ComponentManager::ComponentManager;
};

class Scale2dManager : public ComponentManager<Vec2f, ComponentType::SCALE2D>
{
public:
    using ComponentManager::ComponentManager;
    Index AddComponent(Entity entity) override;

};

class Rotation2dManager : public ComponentManager<degree_t, ComponentType::ROTATION2D>
{
    using ComponentManager::ComponentManager;
};


class Position3dManager : public ComponentManager<Vec3f, ComponentType::POSITION3D>
{
    using ComponentManager::ComponentManager;
};


class Rotation3dManager : public ComponentManager<EulerAngles, ComponentType::ROTATION3D>
{
    using ComponentManager::ComponentManager;
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
    explicit Transform2dManager(EntityManager& entityManager);
    void SetPosition(Entity entity, Vec2f position);
    void SetScale(Entity entity, Vec2f scale);
    void SetRotation(Entity entity, degree_t angles);
    [[nodiscard]] Vec2f GetPosition(Entity entity) const;
    [[nodiscard]] Vec2f GetScale(Entity entity) const;
    [[nodiscard]] degree_t GetRotation(Entity entity) const;
protected:
    Position2dManager positionManager_;
    Scale2dManager scaleManager_;
    Rotation2dManager rotationManager_;
};

class Transform3dManager : public DoubleBufferComponentManager<Mat4f, ComponentType::TRANSFORM3D>,
public OnChangeParentInterface
{
public:
    Transform3dManager(EntityManager& entityManager);
    void Init();
    void SetPosition(Entity entity, Vec3f position);
    void SetScale(Entity entity, Vec3f scale);
    void SetRotation(Entity entity, EulerAngles angles);
    [[nodiscard]]Vec3f GetPosition(Entity entity) const;
    [[nodiscard]] Vec3f GetScale(Entity entity) const;
    [[nodiscard]] EulerAngles GetAngles(Entity entity) const;
    void OnChangeParent(Entity entity, Entity newParent, Entity oldParent) override;
	/**
	 * \brief This function is called by the Dirty Manager
	 */
    void UpdateDirtyComponent(Entity entity) override;
    void Update();
	Index AddComponent(Entity entity) override;
protected:

    void UpdateTransform(Entity entity);
    Position3dManager position3DManager_;
    Scale3dManager scale3DManager_;
    Rotation3dManager rotation3DManager_;
    DirtyManager dirtyManager_;
};

class Transform3dViewer : public DrawImGuiInterface
{
public:
    explicit Transform3dViewer(EntityManager& entityManager, Transform3dManager& transform3dManager);
	void DrawImGui() override;
    void SetSelectedEntity(Entity selectedEntity) { selectedEntity_ = selectedEntity; };
protected:
    Entity selectedEntity_ = INVALID_ENTITY;
    EntityManager& entityManager_;
    Transform3dManager& transform3dManager_;
};





}
