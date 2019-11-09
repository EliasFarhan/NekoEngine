
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
#include <tools/editor_scene.h>
#include <engine/log.h>
#include <tools/neko_editor.h>
#include <utilities/file_utility.h>
#include "utilities//json_utility.h"


namespace neko::editor
{

const char* sceneTmpPath = "data/.tmp.scene";

/*
void EditorSceneManager::ParseSceneJson(json& sceneJson)
{

    //Loading all the scene prefabs
    prefabManager_.ClearPrefabs();
    if (neko::CheckJsonParameter(sceneJson, "prefabPaths", json::value_t::array))
    {
        for (const std::string& prefabPath : sceneJson["prefabPaths"])
        {
            prefabManager_.LoadPrefab(prefabPath);
        }
    }
    neko::SceneManager::ParseSceneJson(sceneJson);
}
 */

EditorSceneManager::EditorSceneManager(NekoEditorExport& editorExport) :
        SfmlBasicSceneManager(editorExport),
        prefabManager_(editorExport.prefabManager),
        bodyDefManager_(editorExport.bodyDef2dManager)
{
    componentSerializeFuncMap_[NekoComponentType::POSITION2D] =
            [this](Entity entity)
            {
                json componentJson;
                const auto& pos = position2dManager_.GetComponent(entity);
                componentJson["position"] = {pos.x, pos.y};
                return componentJson;
            };
    componentSerializeFuncMap_[NekoComponentType::SCALE2D] =
            [this](Entity entity)
            {
                json componentJson;
                const auto& scale = scale2dManager_.GetComponent(entity);
                componentJson["scale"] = {scale.x, scale.y};
                return componentJson;
            };
    componentSerializeFuncMap_[NekoComponentType::ROTATION2D] =
            [this](Entity entity)
            {
                json componentJson;
                componentJson["angle"] = rotation2dManager_.GetComponent(entity);
                return componentJson;
            };
    componentSerializeFuncMap_[NekoComponentType::SPRITE2D] =
            [this](Entity entity)
            {
                return spriteManager_.SerializeComponentJson(entity);
            };
    componentSerializeFuncMap_[NekoComponentType::SPINE_ANIMATION] =
            [this](Entity entity)
            {
                return spineManager_.SerializeComponentJson(entity);
            };
    componentSerializeFuncMap_[NekoComponentType::BODY2D] =
            [this](Entity entity)
            {
                return bodyDefManager_.SerializeComponentJson(entity);
            };

}


json EditorSceneManager::SerializeComponent(neko::Entity entity, neko::NekoComponentType componentType)
{
    json componentJson;
    auto serializeFunc = componentSerializeFuncMap_.find(componentType);
    if(serializeFunc != componentSerializeFuncMap_.end())
    {
        componentJson = serializeFunc->second(entity);
    }
    componentJson["component"] = int(componentType);
    return componentJson;
}

json EditorSceneManager::SerializeEntity(neko::Entity entity)
{


    json entityJson;
    if (!entityManager_.EntityExists(entity))
    {
        return entityJson;
    }
    //TODO scene entity name editor
    //entityJson["name"] = currentScene_.entitiesNames[entity];
    entityJson["entity"] = entity;
    const auto entityHash = entityManager_.GetEntityNameHash(entity);
    entityJson["entityNameHash"] = entityHash;
    //parent entity
    {
        const auto parentEntity = transformManager_.GetParentEntity(entity);
        if (parentEntity != neko::INVALID_ENTITY)
        {
            entityJson["parent"] = int(parentEntity);
        }
        else
        {
            entityJson["parent"] = -1;
        }
    }
    //prefab root entity
    {
        const bool isPrefab = entityManager_.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::PREFAB));
        entityJson["prefab"] = isPrefab;
        if (isPrefab)
        {
            //TODO use prefab Id
            //const auto prefabIndex = GetComponent(entity);
            //entityJson["prefabIndex"] = prefabIndex;
            return entityJson;
        }
    }

    entityJson["components"] = json::array();
    for (auto componentType : neko::GetComponentTypeSet())
    {
        if (entityManager_.HasComponent(entity, neko::EntityMask(componentType)))
        {
            entityJson["components"].push_back(SerializeComponent(entity, componentType));
        }
    }
    return entityJson;
}

json EditorSceneManager::SerializeScene()
{
    json sceneJson;
    sceneJson["entities"] = json::array();
    sceneJson["sceneId"] = currentScene_.sceneId.str();
    sceneJson["sceneName"] = currentScene_.sceneName;

    for (neko::Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
    {
        if (entityManager_.EntityExists(entity))
        {
            auto entityJson = SerializeEntity(entity);
            sceneJson["entities"].push_back(entityJson);
        }
    }
    /*json prefabPaths = json::array();
    for (auto& prefabPath : prefabManager_.GetConstPrefabPaths())
    {
        prefabPaths.push_back(prefabPath);
    }
    sceneJson["prefabPaths"] = prefabPaths;*/
    return sceneJson;
}

void EditorSceneManager::SaveScene(std::string_view path)
{
    logDebug("Saving scene: " + std::string(path));
    auto sceneJson = SerializeScene();
    sceneJson["scenePath"] = path;

    const auto sceneTxt = sceneJson.dump(4);
    neko::WriteStringToFile(path.data(), sceneTxt);
    currentScene_.scenePath = path;
    currentScene_.sceneName = neko::GetFilename(path);
}

void EditorSceneManager::LoadScene(std::string_view path)
{
    auto sceneJson = neko::LoadJson(path);
    ParseSceneJson(sceneJson);
}

void EditorSceneManager::ClearScene() const
{
    for (neko::Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
    {
        transformManager_.SetTransformParent(entity, neko::INVALID_ENTITY);
        entityManager_.DestroyEntity(entity);
    }
}

void EditorSceneManager::SaveCurrentScene()
{
    if (IsCurrentSceneTmp())
    {
        currentScene_.scenePath = sceneTmpPath;
        SaveScene(sceneTmpPath);
    }
    else
    {
        SaveScene(currentScene_.scenePath);
    }
}

bool EditorSceneManager::IsCurrentSceneTmp()
{
    return currentScene_.scenePath.empty() or currentScene_.scenePath == sceneTmpPath;
}

const std::string_view EditorSceneManager::GetSceneTmpPath()
{
    return sceneTmpPath;
}


}
