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

#include <engine/transform.h>
#include "engine/globals.h"
#include "mathematics/transform.h"
#include "imgui.h"
#include "graphics/graphics.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko
{

Index Scale2dManager::AddComponent(Entity entity)
{
	ResizeIfNecessary(components_, entity, Vec2f::one);
	return ComponentManager::AddComponent(entity);
}

Index Scale3dManager::AddComponent(Entity entity)
{
	ResizeIfNecessary(components_, entity, Vec3f::one);
	return ComponentManager::AddComponent(entity);
}


Transform2dManager::Transform2dManager(EntityManager& entityManager) :
	ComponentManager<Mat4f, EntityMask(neko::ComponentType::TRANSFORM2D)>(entityManager),
	positionManager_(entityManager),
	scaleManager_(entityManager),
    rotationManager_(entityManager),
	dirtyManager_(entityManager)
{
    entityManager_.get().RegisterOnChangeParent(this);
    dirtyManager_.RegisterComponentManager(this);
}

void Transform2dManager::SetPosition(Entity entity, Vec2f position)
{
	positionManager_.SetComponent(entity, position);
    dirtyManager_.SetDirty(entity);
}

void Transform2dManager::SetRotation(Entity entity, degree_t angles)
{
	rotationManager_.SetComponent(entity, angles);
	dirtyManager_.SetDirty(entity);
}

Vec2f Transform2dManager::GetPosition(Entity entity) const
{
	return positionManager_.GetComponent(entity);
}

degree_t Transform2dManager::GetRotation(Entity entity) const
{
	return rotationManager_.GetComponent(entity);
}

void Transform2dManager::SetScale(Entity entity, Vec2f scale)
{
    scaleManager_.SetComponent(entity, scale);
    dirtyManager_.SetDirty(entity);
}

Vec2f Transform2dManager::GetScale(Entity entity) const
{
    return scaleManager_.GetComponent(entity);
}

void Transform2dManager::OnChangeParent(Entity entity, [[maybe_unused]] Entity newParent, [[maybe_unused]] Entity oldParent)
{
    //TODO change local transform to not change the global transform when changing parent
    dirtyManager_.SetDirty(entity);
}

void Transform2dManager::UpdateDirtyComponent(Entity entity)
{
    UpdateTransform(entity);
}

void Transform2dManager::Update()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Update Transform");
#endif
    dirtyManager_.UpdateDirtyEntities();
}

void Transform2dManager::UpdateTransform(Entity entity)
{
    Mat4f transform = Mat4f::Identity;
    const auto eulerAngles = EulerAngles(degree_t (0),degree_t (0),-rotationManager_.GetComponent(entity));
    transform = Transform3d::Rotate(transform, eulerAngles);
    const auto scale = scaleManager_.GetComponent(entity);
    transform = Transform3d::Scale(transform, Vec3f(scale, 1.0f));
    const auto position = positionManager_.GetComponent(entity);
    transform = Transform3d::Translate(transform, Vec3f(position, 0.0f));

    const auto parent = entityManager_.get().GetEntityParent(entity);
    if (parent != INVALID_ENTITY)
    {
        transform =  GetComponent(parent) * transform;
    }

    SetComponent(entity, transform);
}

Index Transform2dManager::AddComponent(Entity entity)
{
    positionManager_.AddComponent(entity);
    scaleManager_.AddComponent(entity);
    scaleManager_.SetComponent(entity, Vec2f::one);
    rotationManager_.AddComponent(entity);
    return ComponentManager::AddComponent(entity);
}


Transform3dManager::Transform3dManager(EntityManager& entityManager) :
	DoubleBufferComponentManager(entityManager),
	position3DManager_(entityManager),
	scale3DManager_(entityManager),
	rotation3DManager_(entityManager),
	dirtyManager_(entityManager)
{
	entityManager_.get().RegisterOnChangeParent(this);
	dirtyManager_.RegisterComponentManager(this);
}

void Transform3dManager::Init()
{
	RendererLocator::get().RegisterSyncBuffersFunction(this);
}

void Transform3dManager::UpdateTransform(Entity entity)
{
    Mat4f transform = Mat4f::Identity;
	transform = Transform3d::Rotate(transform, rotation3DManager_.GetComponent(entity));
	transform = Transform3d::Scale(transform, scale3DManager_.GetComponent(entity));
    transform = Transform3d::Translate(transform, position3DManager_.GetComponent(entity));

	const auto parent = entityManager_.get().GetEntityParent(entity);
	if (parent != INVALID_ENTITY)
	{
		transform =  GetComponent(parent) * transform;
	}
	
    SetComponent(entity, transform);
}

Transform3dViewer::Transform3dViewer(EntityManager& entityManager, Transform3dManager& transform3dManager) :
	entityManager_(entityManager),
	transform3dManager_(transform3dManager)
{
}

void Transform3dViewer::DrawImGui()
{
	if(selectedEntity_ == INVALID_ENTITY)
		return;
	if(entityManager_.HasComponent(selectedEntity_, static_cast<EntityMask>(ComponentType::TRANSFORM3D)))
	{
		auto position = transform3dManager_.GetPosition(selectedEntity_);
		if (ImGui::InputFloat3("Position", &position[0]))
		{
			transform3dManager_.SetPosition(selectedEntity_, position);
		}
		auto scale = transform3dManager_.GetScale(selectedEntity_);
		if(ImGui::InputFloat3("Scale", &scale[0]))
		{
			transform3dManager_.SetScale(selectedEntity_, scale);
		}
		const auto eulerAngles = transform3dManager_.GetAngles(selectedEntity_);
		float angles[3] = { eulerAngles.x.value(), eulerAngles.y.value(), eulerAngles.z.value() };
		if(ImGui::InputFloat3("Euler Angles", &angles[0]))
		{
			const EulerAngles newEulerAngles = EulerAngles(degree_t(angles[0]), degree_t(angles[1]), degree_t(angles[2]));
			transform3dManager_.SetRotation(selectedEntity_, newEulerAngles);
		}
	}
}

void Transform3dManager::SetPosition(Entity entity, Vec3f position)
{
	position3DManager_.SetComponent(entity, position);
	dirtyManager_.SetDirty(entity);
}

void Transform3dManager::SetScale(Entity entity, Vec3f scale)
{
	scale3DManager_.SetComponent(entity, scale);
	dirtyManager_.SetDirty(entity);
}

void Transform3dManager::SetRotation(Entity entity, EulerAngles angles)
{
	rotation3DManager_.SetComponent(entity, angles);
	dirtyManager_.SetDirty(entity);
}

Vec3f Transform3dManager::GetPosition(Entity entity) const
{
	return position3DManager_.GetComponent(entity);
}

Vec3f Transform3dManager::GetScale(Entity entity) const
{
	return scale3DManager_.GetComponent(entity);
}

EulerAngles Transform3dManager::GetAngles(Entity entity) const
{
	return rotation3DManager_.GetComponent(entity);
}

void Transform3dManager::OnChangeParent(Entity entity, [[maybe_unused]] Entity newParent, [[maybe_unused]] Entity oldParent)
{
	dirtyManager_.SetDirty(entity);
}

void Transform3dManager::UpdateDirtyComponent(Entity entity)
{
	UpdateTransform(entity);
}

void Transform3dManager::Update()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Update Transform");
#endif
	dirtyManager_.UpdateDirtyEntities();
}

Index Transform3dManager::AddComponent(Entity entity)
{
	position3DManager_.AddComponent(entity);
	scale3DManager_.AddComponent(entity);
	scale3DManager_.SetComponent(entity, Vec3f::one);
	rotation3DManager_.AddComponent(entity);
	return DoubleBufferComponentManager::AddComponent(entity);
}
}
