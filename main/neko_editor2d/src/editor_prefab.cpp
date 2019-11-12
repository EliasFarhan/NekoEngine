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
#include <tools/engine_export.h>
#include <sfml_engine/physics.h>
#include <tools/physics_editor.h>
#include <tools/editor_prefab.h>
#include <tools/editor_scene.h>
#include <utilities/file_utility.h>
#include <engine/log.h>
#include "sfml_engine/transform.h"
#include "sfml_engine/sprite.h"
#include "sfml_engine/spine.h"

namespace neko::editor
{


void EditorPrefabManager::SaveCurrentPrefab() const
{
	if (currentPrefabId_ == INVALID_PREFAB_ID)
	{
		logDebug("[Warning] Trying to save current prefab while none is defined");
		return;
	}
	const Prefab& currentPrefab = GetCurrentPrefab();
	logDebug("Save Current Prefab at: " + std::string(currentPrefab.prefabPath));
	json prefabJson = editorSceneManager_.SerializeScene();
	prefabJson.erase("sceneName");
	//Remove the prefab flags on the first entity
	for (auto& entityJson : prefabJson["entities"])
	{
		if (entityJson["entity"] == 0)
		{
			entityJson["prefab"] = false;
		}
	}
	prefabJson["prefabId"] = currentPrefab.id.str();
	const auto prefabTxt = prefabJson.dump(4);
	WriteStringToFile(currentPrefab.prefabPath, prefabTxt);
}

neko::Index EditorPrefabManager::CreatePrefabFromEntity(neko::Entity entity)
{
	//const auto newIndex = neko::Index(prefabJsons_.size());
	/*auto& sceneManager = dynamic_cast<EditorSceneManager&>(sceneManager_);

	json prefabJson;
	prefabJson["entities"] = json::array();
	neko::Entity rootEntity = entity;
	entityManager_.AddComponentType(entity, neko::EntityMask(neko::NekoComponentType::PREFAB));
	{
		auto entityJson = sceneManager.SerializeEntity(rootEntity);
		entityJson["parent"] = -1;//No parent in prefab
		entityJson["entity"] = 0;//Root entity is 0
		entityJson["prefab"] = false;
		prefabJson["entities"].push_back(entityJson);

	}

	const neko::Entity currentParentEntity = rootEntity;
	std::function<void(neko::Entity)> updateInstanceEntityFunc = [&](neko::Entity parent)
	{
		neko::Entity currentEntity = transformManager_.FindNextChild(parent);
		while (currentEntity != neko::INVALID_ENTITY)
		{
			auto entityJson = sceneManager.SerializeEntity(currentEntity);
			entityJson["entity"] = currentEntity - rootEntity;//TODO negative entity
			entityJson["parent"] = parent - rootEntity;//TODO negative parent
			prefabJson["entities"].push_back(entityJson);
			updateInstanceEntityFunc(currentEntity);
			currentEntity = transformManager_.FindNextChild(parent, currentEntity);
		}
	};
	updateInstanceEntityFunc(currentParentEntity);
	const std::function<void(neko::Entity)> removeChildrenEntityFunc = [&](neko::Entity parent)
	{
		neko::Entity currentEntity = transformManager_.FindNextChild(parent);
		while (currentEntity != neko::INVALID_ENTITY)
		{
			entityManager_.DestroyEntity(currentEntity);
			currentEntity = transformManager_.FindNextChild(parent, currentEntity);
		}
	};
	removeChildrenEntityFunc(currentParentEntity);
	prefabJsons_.push_back(prefabJson);
	prefabPaths_.push_back("");
	currentPrefabIndex_ = prefabJsons_.size() - 1;
*/
	return INVALID_INDEX;

}

EditorPrefabManager::EditorPrefabManager(NekoEditorExport& editorExport) :
	PrefabManager(editorExport.sceneManager),
	entityManager_(editorExport.entityManager),
	transformManager_(editorExport.transform2dManager),
	position2dManager_(editorExport.position2dManager),
	spriteManager_(editorExport.spriteManager),
	boxColliderDefManager_(editorExport.boxColliderDefManager),
	circleColliderDefManager_(editorExport.circleColliderDefManager),
	spineManager_(editorExport.spineManager),
	editorSceneManager_(editorExport.sceneManager)
{
}

void EditorPrefabManager::SetCurrentPrefab(const Prefab& prefab)
{
	currentPrefabId_ = prefab.id;
	prefabMap_[currentPrefabId_] = prefab;
	
}

const Prefab& EditorPrefabManager::GetCurrentPrefab() const
{
	return prefabMap_.at(currentPrefabId_);
}


sf::FloatRect EditorPrefabManager::CalculatePrefabBound()
{
	const auto spriteEntityMask =
		EntityMask(NekoComponentType::SPRITE2D) |
		EntityMask(NekoComponentType::TRANSFORM2D);
	const auto boxEntityMask =
		EntityMask(NekoComponentType::BOX_COLLIDER2D) |
		EntityMask(NekoComponentType::TRANSFORM2D);
	const auto circleEntityMask =
		EntityMask(NekoComponentType::CIRCLE_COLLIDER2D) |
		EntityMask(NekoComponentType::TRANSFORM2D);
	const auto spineEntityMask =
		EntityMask(NekoComponentType::SPINE_ANIMATION) |
		EntityMask(NekoComponentType::TRANSFORM2D);

	sf::FloatRect entityRect;
	if (entityManager_.HasComponent(0u,
		EntityMask(NekoComponentType::POSITION2D)))
	{
		const auto pos = unit2pixel(position2dManager_.GetComponent(0u));
		entityRect.left = pos.x;
		entityRect.top = pos.y;
	}
	auto updateEntityRect = [&entityRect](const sf::FloatRect& rect)
	{
		if (rect.left < entityRect.left)
		{
			entityRect.width += entityRect.left - rect.left;
			entityRect.left = rect.left;
		}
		if (rect.top < entityRect.top)
		{
			entityRect.height += entityRect.top - rect.top;
			entityRect.top = rect.top;
		}
		if (rect.left + rect.width > entityRect.left + entityRect.width)
		{
			const auto rectRight = rect.left + rect.width;
			const auto entityRectRight = entityRect.left + entityRect.width;
			entityRect.width += rectRight - entityRectRight;
		}
		if (rect.top + rect.height > entityRect.top + entityRect.height)
		{
			const auto rectBottom = rect.top + rect.height;
			const auto entityRectBottom = entityRect.top + entityRect.height;
			entityRect.height += rectBottom - entityRectBottom;
		}
	};
	auto includeEntityRect = [&](Entity entity)
	{
		const auto pos = unit2pixel(position2dManager_.GetComponent(entity));
		const auto transform = transformManager_.CalculateTransform(entity);
		if (entityManager_.HasComponent(entity, spriteEntityMask))
		{
			auto& sprite = spriteManager_.GetComponent(entity).sprite;
			const auto rect = transform.transformRect(sprite.getGlobalBounds());
			updateEntityRect(rect);

		}
		if (entityManager_.HasComponent(entity, boxEntityMask))
		{
			const auto& box = boxColliderDefManager_.GetComponent(entity);
			const sf::FloatRect rect(pos + unit2pixel(box.size / 2.0f), unit2pixel(box.size));
			const auto globalRect = transform.transformRect(rect);
			updateEntityRect(globalRect);
		}
		if (entityManager_.HasComponent(entity, circleEntityMask))
		{
			const auto& circleCollider = circleColliderDefManager_.GetComponent(entity);
			const auto size = box2d::meter2pixel(
				b2Vec2(circleCollider.shapeDef.m_radius, circleCollider.shapeDef.m_radius));
			const sf::FloatRect rect(pos + size / 2.0f, size);
			const auto globalRect = transform.transformRect(rect);
			updateEntityRect(globalRect);
		}
		if (entityManager_.HasComponent(entity, spineEntityMask))
		{
			const auto& spineComp = spineManager_.GetComponent(entity);
			if (spineComp.skeletonDrawable != nullptr)
			{
				const auto rect = transform.transformRect(
					spineComp.skeletonDrawable->vertexArray->getBounds());
				updateEntityRect(rect);

			}
		}


	};
	std::function<void(Entity)> includeEntityRectRecursive = [&](Entity parent)
	{
		Entity currentEntity = transformManager_.FindNextChild(parent);
		while (currentEntity != INVALID_ENTITY)
		{
			includeEntityRect(currentEntity);
			includeEntityRectRecursive(currentEntity);
			currentEntity = transformManager_.FindNextChild(parent, currentEntity);
		}
	};
	includeEntityRect(0u);
	includeEntityRectRecursive(0u);
	return entityRect;

}

bool EditorPrefabManager::IsCurrentPrefabValid() const
{
	return currentPrefabId_ != INVALID_PREFAB_ID;
}
}
