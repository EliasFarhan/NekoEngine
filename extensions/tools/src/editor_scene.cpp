
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


namespace editor
{

const char* sceneTmpPath = "data/.tmp.scene";

void EditorSceneManager::ParseComponentJson(json& componentJson, neko::Entity entity)
{
    auto& entityManager = nekoEditor_.GetEntityManager();
    neko::NekoComponentType componentType = componentJson["component"];
    switch (componentType)
    {
        case neko::NekoComponentType::POSITION2D:
        {
            auto& positionManager = nekoEditor_.GetPositionManager();
            auto pos = neko::GetVectorFromJson(componentJson, "position");
            positionManager.AddComponent(entityManager, entity);
            positionManager.SetComponent(entity, pos);
            break;
        }
        case neko::NekoComponentType::SCALE2D:
        {
            auto& scaleManager = nekoEditor_.GetScaleManager();
            auto scale = neko::GetVectorFromJson(componentJson, "scale");
            scaleManager.AddComponent(entityManager, entity);
            scaleManager.SetComponent(entity, scale);
            break;
        }
        case neko::NekoComponentType::ROTATION2D:
        {
            auto& angleManager = nekoEditor_.GetRotationManager();
            angleManager.AddComponent(entityManager, entity);
            angleManager.SetComponent(entity, componentJson["angle"]);
            break;
        }
        case neko::NekoComponentType::SPRITE2D:
        {
            auto& spriteManager = nekoEditor_.GetSpriteManager();
            spriteManager.AddComponent(entityManager, entity);
            spriteManager.ParseComponentJson(componentJson, entity);
            break;
        }
        case neko::NekoComponentType::SPINE_ANIMATION:
        {
            auto& spineManager = nekoEditor_.GetSpineManager();
            spineManager.AddComponent(entityManager, entity);
            spineManager.ParseComponentJson(componentJson, entity);
            break;
        }
        case neko::NekoComponentType::BODY2D:
        {
            auto& bodyDefManager = nekoEditor_.GetBodyDefManager();
            bodyDefManager.AddComponent(entityManager, entity);
            bodyDefManager.ParseComponentJson(componentJson, entity);
            break;
        }
        case neko::NekoComponentType::BOX_COLLIDER2D:
            break;
        case neko::NekoComponentType::CONVEX_SHAPE2D:
            break;
        default:
            break;
    }
}

void EditorSceneManager::ParseSceneJson(json& sceneJson)
{
	//Loading all the scene prefabs
	auto& prefabManager = nekoEditor_.GetPrefabManager();
	prefabManager.ClearPrefabs();
	if (neko::CheckJsonParameter(sceneJson, "prefabPaths", json::value_t::array))
	{
		for (const std::string& prefabPath : sceneJson["prefabPaths"])
		{
			prefabManager.LoadPrefab(prefabPath);
		}
	}
	neko::SceneManager::ParseSceneJson(sceneJson);

	
}

void EditorSceneManager::ParseEntityJson(json& entityJson)
{
    neko::Entity entity = neko::INVALID_ENTITY;
    if (neko::CheckJsonExists(entityJson, "entity") and neko::IsJsonValueNumeric(entityJson["entity"]))
    {
        entity = entityJson["entity"];
    }
    if (entity == neko::INVALID_ENTITY)
    {
        logDebug("[Error] Scene loader entity with no entity nmb");
        return;
    }
	if (neko::CheckJsonNumber(entityJson, "parent"))
	{
		const int parentEntity = entityJson["parent"];

		logDebug("Parsing entity: " + std::to_string(entity) + " with parent: " + std::to_string(parentEntity));
		if (parentEntity >= 0)
		{
			auto& transformManager = nekoEditor_.GetTransformManager();
			transformManager.SetTransformParent(entity, neko::Entity(parentEntity));
		}
	}
	if (neko::CheckJsonParameter(entityJson, "name", json::value_t::string))
	{
		ResizeIfNecessary(currentScene_.entitiesNames, entity, std::string());
		currentScene_.entitiesNames[entity] = entityJson["name"];

	}
	auto& entityManager = nekoEditor_.GetEntityManager();
	if (neko::CheckJsonParameter(entityJson, "prefab", json::value_t::boolean))
	{
		if (entityJson["prefab"])
		{
			auto& prefabManager = nekoEditor_.GetPrefabManager();
			if (neko::CheckJsonNumber(entityJson, "prefabIndex"))
			{
				const neko::Index prefabIndex = entityJson["prefabIndex"];
				prefabManager.InstantiatePrefab(prefabIndex, entityManager);
			}
			return;
		}
	}
    entityManager.CreateEntity(entity);
   

	

    if (neko::CheckJsonParameter(entityJson, "components", json::value_t::array))
    {
        for (auto& componentJson : entityJson["components"])
        {
            ParseComponentJson(componentJson, entity);
        }
    }
}


json EditorSceneManager::SerializeComponent(neko::Entity entity, neko::NekoComponentType componentType)
{
    json componentJson;
    switch (componentType)
    {

        case neko::NekoComponentType::POSITION2D:
        {
            auto& positionManager = nekoEditor_.GetPositionManager();
            const auto& pos = positionManager.GetConstComponent(entity);
            componentJson["position"] = {pos.x, pos.y};
            break;
        }
        case neko::NekoComponentType::SCALE2D:
        {
            auto& scaleManager = nekoEditor_.GetScaleManager();
            const auto& scale = scaleManager.GetConstComponent(entity);
            componentJson["scale"] = {scale.x, scale.y};
            break;
        }
        case neko::NekoComponentType::ROTATION2D:
        {
            auto& angleManager = nekoEditor_.GetRotationManager();
            componentJson["angle"] = angleManager.GetComponent(entity);
            break;
        }
        case neko::NekoComponentType::SPRITE2D:
        {
            auto& spriteManager = nekoEditor_.GetSpriteManager();
            componentJson = spriteManager.SerializeComponentJson(entity);
            break;
        }
        case neko::NekoComponentType::SPINE_ANIMATION:
        {
            auto& spineManager = nekoEditor_.GetSpineManager();
            componentJson = spineManager.SerializeComponentJson(entity);
            break;
        }
        case neko::NekoComponentType::BODY2D:
            break;
        case neko::NekoComponentType::BOX_COLLIDER2D:
            break;
        case neko::NekoComponentType::CONVEX_SHAPE2D:
            break;
        default:
            break;
    }

    componentJson["component"] = int(componentType);
    return componentJson;
}

json EditorSceneManager::SerializeEntity(neko::Entity entity)
{

    auto& entityManager = nekoEditor_.GetEntityManager();

    json entityJson;
    if (!entityManager.EntityExists(entity))
    {
        return entityJson;
    }
    entityJson["name"] = currentScene_.entitiesNames[entity];
    entityJson["entity"] = entity;
    //parent entity
    {
        auto& transformManager = nekoEditor_.GetTransformManager();
        const auto parentEntity = transformManager.GetParentEntity(entity);
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
		const bool isPrefab = entityManager.HasComponent(entity, neko::EntityMask(neko::NekoComponentType::PREFAB));
		entityJson["prefab"] = isPrefab;
		if(isPrefab)
		{
			const auto prefabIndex = GetConstComponent(entity);
			entityJson["prefabIndex"] = prefabIndex;
			return entityJson;
		}
    }
	
    entityJson["components"] = json::array();
    for (auto componentType : neko::GetComponentTypeSet())
    {
        if (entityManager.HasComponent(entity, neko::EntityMask(componentType)))
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
    sceneJson["sceneName"] = currentScene_.sceneName;
    auto& entityManager = nekoEditor_.GetEntityManager();

    for (neko::Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.EntityExists(entity))
        {
            auto entityJson = SerializeEntity(entity);
            sceneJson["entities"].push_back(entityJson);
        }
    }
	auto& prefabManager = nekoEditor_.GetPrefabManager();
	json prefabPaths = json::array();
	for(auto& prefabPath : prefabManager.GetConstPrefabPaths())
	{
		prefabPaths.push_back(prefabPath);
	}
	sceneJson["prefabPaths"] = prefabPaths;
    return sceneJson;
}

void EditorSceneManager::SaveScene(std::string_view path)
{
    logDebug("Saving scene: "+std::string(path));
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

void EditorSceneManager::ClearScene()
{
    auto& entityManager = nekoEditor_.GetEntityManager();
    auto& transformManager = nekoEditor_.GetTransformManager();
    for (neko::Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        transformManager.SetTransformParent(entity, neko::INVALID_ENTITY);
        entityManager.DestroyEntity(entity);
    }
}

void EditorSceneManager::SaveCurrentScene()
{
    if(IsCurrentSceneTmp())
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