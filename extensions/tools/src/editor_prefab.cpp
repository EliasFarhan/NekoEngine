//
// Created by efarhan on 8/13/19.
//

#include <tools/editor_prefab.h>
#include <tools/editor_scene.h>
#include <utilities/file_utility.h>
#include <engine/log.h>
#include <tools/neko_editor.h>

namespace editor
{

const char* prefabTmpPath = "data/.tmp.prefab";

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

    const auto prefabTxt = prefabJson.dump(4);
    if (currentPrefabIndex_ != neko::INVALID_INDEX)
    {
        prefabJsons_[currentPrefabIndex_] = prefabJson;
    }
    else
    {
        currentPrefabIndex_ = prefabJsons_.size();
        prefabJsons_.push_back(prefabJson);
        prefabPaths_.push_back(path.data());
    }
    neko::WriteStringToFile(path.data(), prefabTxt);
}

neko::Index EditorPrefabManager::CreatePrefabFromEntity(neko::Entity entity)
{
    auto newIndex = neko::Index(prefabJsons_.size());
    auto& sceneManager = dynamic_cast<EditorSceneManager&>(sceneManager_);
    auto& transformManager = nekoEditor_.GetTransformManager();
    auto& entityManager = nekoEditor_.GetEntityManager();

    json prefabJson;
    prefabJson["entities"] = json::array();
    neko::Entity rootEntity = entity;
    {
        auto entityJson = sceneManager.SerializeEntity(rootEntity);
        entityJson["parent"] = -1;//No parent in prefab
        entityJson["entity"] = 0;//Root entity is 0
        prefabJson["entities"].push_back(entityJson);
    }

    neko::Entity currentParentEntity = rootEntity;
    std::function<void(neko::Entity)> updateInstanceEntityfunc = [&](neko::Entity parent)
    {
        neko::Entity currentEntity = transformManager.FindNextChild(parent);
        while (currentEntity != neko::INVALID_ENTITY)
        {
            auto entityJson = sceneManager.SerializeEntity(currentEntity);
            entityJson["entity"] = currentEntity - rootEntity;//TODO negative entity
            entityJson["parent"] = parent - rootEntity;//TODO negative parent
            prefabJson["entities"].push_back(entityJson);
            updateInstanceEntityfunc(currentEntity);
            currentEntity = transformManager.FindNextChild(parent, currentEntity);

        }
    };
    updateInstanceEntityfunc(currentParentEntity);
    prefabJsons_.push_back(prefabJson);
    prefabPaths_.push_back("");

    return newIndex;

}

EditorPrefabManager::EditorPrefabManager(NekoEditor& editor) : nekoEditor_(editor),
                                                               PrefabManager(editor.GetSceneManager())
{

}

void EditorPrefabManager::SaveCurrentPrefab()
{
    if (IsCurrentPrefabTmp())
    {
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
    return currentPrefabPath_.empty() or currentPrefabPath_ == prefabTmpPath;
}

sf::FloatRect EditorPrefabManager::CalculatePrefabBound()
{
    auto& entityManager = nekoEditor_.GetEntityManager();
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
    auto& transformManager = nekoEditor_.GetTransformManager();

    sf::FloatRect entityRect;
    if (entityManager.HasComponent(0u,
            neko::EntityMask(neko::NekoComponentType::POSITION2D)))
    {
        const auto pos = neko::unit2pixel(transformManager.GetPositionManager().GetConstComponent(0u));
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

        auto transform = transformManager.CalculateTransform(entity);
        if (entityManager.HasComponent(entity, spriteEntityMask))
        {
            auto& spriteManager = nekoEditor_.GetSpriteManager();
            auto& sprite = spriteManager.GetConstComponent(entity).sprite;
            const auto rect = transform.transformRect(sprite.getGlobalBounds());
            updateEntityRect(rect);

        }
        if(entityManager.HasComponent(entity, boxEntityMask))
        {
            auto& boxColliderManager = nekoEditor_.GetColliderDefManager().GetBoxColliderDefManager();
            const auto& box = boxColliderManager.GetConstComponent(entity);
            const auto rect = transform.transformRect(box.shape.getGlobalBounds());
            updateEntityRect(rect);
        }
        if(entityManager.HasComponent(entity, circleEntityMask))
        {
            auto& circleColliderDefManager = nekoEditor_.GetColliderDefManager().GetCircleColliderDefManager();
            const auto& circleCollider = circleColliderDefManager.GetConstComponent(entity);
            const auto rect = transform.transformRect(circleCollider.shape.getGlobalBounds());
            updateEntityRect(rect);
        }
        if(entityManager.HasComponent(entity, spineEntityMask))
        {
            auto& spineManager = nekoEditor_.GetSpineManager();
            const auto& spineComp = spineManager.GetConstComponent(entity);
            if(spineComp.skeletonDrawable != nullptr)
            {
                const auto rect = transform.transformRect(
                        spineComp.skeletonDrawable->vertexArray->getBounds());
                updateEntityRect(rect);

            }
        }


    };
    std::function<void(neko::Entity)> includeEntityRectRecursive = [&](neko::Entity parent)
    {
        neko::Entity currentEntity = transformManager.FindNextChild(parent);
        while (currentEntity != neko::INVALID_ENTITY)
        {
            includeEntityRect(currentEntity);
            includeEntityRectRecursive(currentEntity);
            currentEntity = transformManager.FindNextChild(parent, currentEntity);
        }
    };
    includeEntityRect(0u);
    includeEntityRectRecursive(0u);
    return entityRect;

}
}