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

#include <engine/transform.h>
#include "engine/globals.h"
#include "mathematics/transform.h"
#include "imgui.h"
#include "graphics/graphics.h"

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

Transform3dManager::Transform3dManager(EntityManager& entityManager) :
	DoubleBufferComponentManager(entityManager),
	position3DManager_(entityManager),
	scale3DManager_(entityManager),
	rotation3DManager_(entityManager),
	dirtyManager_(entityManager)
{
	entityManager_.RegisterOnChangeParent(this);
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

	const auto parent = entityManager_.GetEntityParent(entity);
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

void Transform3dManager::OnChangeParent(Entity entity, Entity newParent, Entity oldParent)
{
	dirtyManager_.SetDirty(entity);
}

void Transform3dManager::UpdateDirtyComponent(Entity entity)
{
	UpdateTransform(entity);
}

void Transform3dManager::Update()
{
	dirtyManager_.UpdateDirtyEntities();
}

Index Transform3dManager::AddComponent(Entity entity)
{
	position3DManager_.AddComponent(entity);
	scale3DManager_.AddComponent(entity);
	rotation3DManager_.AddComponent(entity);
	return DoubleBufferComponentManager::AddComponent(entity);
}
}
