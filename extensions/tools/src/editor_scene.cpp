#include <tools/editor_scene.h>
#include <engine/log.h>
#include <tools/neko_editor.h>

namespace editor
{
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
    auto& entityManager = nekoEditor_.GetEntityManager();
    entityManager.CreateEntity(entity);
    if (neko::CheckJsonParameter(entityJson, "name", json::value_t::string))
    {
        ResizeIfNecessary(currentScene_.entitiesNames, entity);
        currentScene_.entitiesNames[entity] = entityJson["name"];

    }

    if (neko::CheckJsonParameter(entityJson, "components", json::value_t::array))
    {
        for (auto& componentJson : entityJson["components"])
        {
            ParseComponentJson(componentJson, entity);
        }
    }
}

void EditorSceneManager::ParseComponentJson(json& componentJson, neko::Entity entity)
{

}

json EditorSceneManager::SerializeEntity(neko::Entity entity)
{

    auto& entityManager = nekoEditor_.GetEntityManager();

    json entityJson;
    if(entityManager.EntityExists(entity))
    {
        entityJson["name"] = currentScene_.entitiesNames[entity];
        entityJson["entity"] = entity;
    }
    return entityJson;
}

json EditorSceneManager::SerializeScene()
{
    json sceneJson;
    sceneJson["entities"] = json::array();

    auto& entityManager = nekoEditor_.GetEntityManager();

    for(neko::Entity entity = 0; entity < entityManager.GetEntitiesSize();entity++ )
    {
        if(entityManager.EntityExists(entity))
        {
            auto entityJson = SerializeEntity(entity);
            sceneJson["entities"].push_back(entityJson);
        }
    }
    return sceneJson;
}
}