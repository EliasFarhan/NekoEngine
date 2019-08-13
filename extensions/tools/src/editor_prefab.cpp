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

void EditorPrefabManager::SaveCurrentPrefab(const std::string_view path)
{
    logDebug("Save Current Prefab at: "+std::string(path));
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
    std::function<void(neko::Entity)> func = [&](neko::Entity parent)
    {
        neko::Entity currentEntity = transformManager.FindNextChild(parent);
        while (currentEntity != neko::INVALID_ENTITY)
        {
            auto entityJson = sceneManager.SerializeEntity(currentEntity);
            entityJson["entity"] = currentEntity - rootEntity;//TODO negative entity
            entityJson["parent"] = parent - rootEntity;//TODO negative parent
            prefabJson["entities"].push_back(entityJson);
            func(currentEntity);
            currentEntity = transformManager.FindNextChild(parent, currentEntity);

        }
    };
    func(currentParentEntity);
    prefabJsons_.push_back(prefabJson);
    prefabPaths_.push_back("");

    return newIndex;

}

EditorPrefabManager::EditorPrefabManager(NekoEditor& editor) : nekoEditor_(editor),
                                                               PrefabManager(editor.GetSceneManager())
{

}
}