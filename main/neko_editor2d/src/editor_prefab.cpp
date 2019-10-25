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
#include <tools/editor_prefab.h>
#include <tools/editor_scene.h>
#include <utilities/file_utility.h>
#include <engine/log.h>
#include <tools/neko_editor.h>
#include "sfml_engine/transform.h"

namespace neko::editor
{


const std::string& EditorPrefabManager::GetCurrentPrefabPath() const
{
	return currentPrefabPath_;
}

void EditorPrefabManager::SetCurrentPrefabPath(const std::string& currentPrefabPath)
{
	currentPrefabPath_ = currentPrefabPath;
}

neko::Index EditorPrefabManager::GetCurrentPrefabIndex() const
{
	return currentPrefabIndex_;
}

void EditorPrefabManager::SetCurrentPrefabIndex(neko::Index currentPrefabIndex)
{
	currentPrefabIndex_ = currentPrefabIndex;
}

void EditorPrefabManager::SavePrefab(const std::string_view path)
{
	logDebug("Save Current Prefab at: " + std::string(path));
	auto& sceneManager = dynamic_cast<EditorSceneManager&>(sceneManager_);
	json prefabJson = sceneManager.SerializeScene();
	prefabJson.erase("sceneName");
	for (auto& entityJson : prefabJson["entities"])
	{
		if (entityJson["entity"] == 0)
		{
			entityJson["prefab"] = false;
		}
	}

	const auto prefabTxt = prefabJson.dump(4);
	if (currentPrefabIndex_ != neko::INVALID_INDEX and currentPrefabIndex_ < prefabJsons_.size())
	{
		prefabJsons_[currentPrefabIndex_] = prefabJson;
	}
	else
	{
		currentPrefabIndex_ = neko::Index(prefabJsons_.size());
		prefabJsons_.push_back(prefabJson);
		prefabPaths_.push_back(path.data());
	}
	neko::WriteStringToFile(path.data(), prefabTxt);
}

neko::Index EditorPrefabManager::CreatePrefabFromEntity(neko::Entity entity)
{
	const auto newIndex = neko::Index(prefabJsons_.size());
	auto& sceneManager = dynamic_cast<EditorSceneManager&>(sceneManager_);

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
	return newIndex;

}

EditorPrefabManager::EditorPrefabManager(NekoEditorExport& editorExport) :
	PrefabManager(editorExport.sceneManager),
	entityManager_(editorExport.entityManager),
	transformManager_(editorExport.transform2dManager),
	position2dManager_(editorExport.position2dManager),
	spriteManager_(editorExport.spriteManager),
	boxColliderDefManager_(editorExport.boxColliderDefManager_),
	circleColliderDefManager_(editorExport.circleColliderDefManager_),
	spineManager_(editorExport.spineManager)
{
}

void EditorPrefabManager::SaveCurrentPrefab()
{
	if (IsCurrentPrefabTmp())
	{
		if (currentPrefabIndex_ == neko::INVALID_INDEX)
			currentPrefabIndex_ = 0;
		const std::string prefabTmpPath = "data/.tmp" + std::to_string(currentPrefabIndex_) + ".prefab";

		const auto& scene = sceneManager_.GetCurrentScene();
		const std::string_view scenePath = scene.scenePath;

		if (!scenePath.empty())
		{
			json sceneJson = neko::LoadJson(scenePath);
			const auto prefabIndex = GetCurrentPrefabIndex();
			if (prefabIndex < sceneJson.size())
			{
				sceneJson["prefabPaths"][prefabIndex] = prefabTmpPath;
				const auto sceneTxt = sceneJson.dump(4);
				neko::WriteStringToFile(scenePath.data(), sceneTxt);
				logDebug("Update scene with prefab path: " + prefabTmpPath);
			}
		}

		SetCurrentPrefabPath(prefabTmpPath);
		SavePrefab(prefabTmpPath);
	}
	else
	{
		SavePrefab(currentPrefabPath_);
	}
}

bool EditorPrefabManager::IsCurrentPrefabTmp()
{
	const std::string prefabTmpPath = "data/.tmp" + std::to_string(currentPrefabIndex_) + ".prefab";
	return currentPrefabPath_.empty() or currentPrefabPath_ == prefabTmpPath;
}

sf::FloatRect EditorPrefabManager::CalculatePrefabBound()
{
	const auto spriteEntityMask =
		neko::EntityMask(neko::NekoComponentType::SPRITE2D) |
		neko::EntityMask(neko::NekoComponentType::TRANSFORM2D);
	const auto boxEntityMask =
		neko::EntityMask(neko::NekoComponentType::BOX_COLLIDER2D) |
		neko::EntityMask(neko::NekoComponentType::TRANSFORM2D);
	const auto circleEntityMask =
		neko::EntityMask(neko::NekoComponentType::CIRCLE_COLLIDER2D) |
		neko::EntityMask(neko::NekoComponentType::TRANSFORM2D);
	const auto spineEntityMask =
		neko::EntityMask(neko::NekoComponentType::SPINE_ANIMATION) |
		neko::EntityMask(neko::NekoComponentType::TRANSFORM2D);

	sf::FloatRect entityRect;
	if (entityManager_.HasComponent(0u,
		neko::EntityMask(neko::NekoComponentType::POSITION2D)))
	{
		const auto pos = neko::unit2pixel(position2dManager_.GetComponent(0u));
		entityRect.left = pos.x;
		entityRect.top = pos.y;
	}
	auto updateEntityRect = [&](const sf::FloatRect& rect)
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
	auto includeEntityRect = [&](neko::Entity entity)
	{

		auto transform = transformManager_.CalculateTransform(entity);
		if (entityManager_.HasComponent(entity, spriteEntityMask))
		{
			auto& sprite = spriteManager_.GetComponent(entity).sprite;
			const auto rect = transform.transformRect(sprite.getGlobalBounds());
			updateEntityRect(rect);

		}
		if (entityManager_.HasComponent(entity, boxEntityMask))
		{
			const auto& box = boxColliderDefManager_.GetComponent(entity);
			const auto rect = transform.transformRect(box.shape.getGlobalBounds());
			updateEntityRect(rect);
		}
		if (entityManager_.HasComponent(entity, circleEntityMask))
		{
			const auto& circleCollider = circleColliderDefManager_.GetComponent(entity);
			const auto rect = transform.transformRect(circleCollider.shape.getGlobalBounds());
			updateEntityRect(rect);
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
	std::function<void(neko::Entity)> includeEntityRectRecursive = [&](neko::Entity parent)
	{
		neko::Entity currentEntity = transformManager_.FindNextChild(parent);
		while (currentEntity != neko::INVALID_ENTITY)
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
}