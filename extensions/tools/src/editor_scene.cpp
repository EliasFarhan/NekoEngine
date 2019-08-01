#include <tools/editor_scene.h>
#include <engine/log.h>
#include <tools/neko_editor.h>


namespace editor
{


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
        case neko::NekoComponentType::ANGLE2D:
        {
            auto& angleManager = nekoEditor_.GetAngleManager();
            angleManager.AddComponent(entityManager, entity);
            angleManager.SetComponent(entity, componentJson["angle"]);
            break;
        }
        case neko::NekoComponentType::SPRITE2D:
        {
            auto& spriteManager = nekoEditor_.GetSpriteManager();
            auto& textureManager = nekoEditor_.GetTextureManager();
            spriteManager.AddComponent(entityManager, entity);
            std::string textureName = componentJson["texture"];
            const auto textureId = textureManager.LoadTexture(textureName);
            spriteManager.CopyTexture(textureId, entity, 1);
            spriteManager.CopyLayer(componentJson["layer"], entity, 1);
            const auto origin = neko::GetVectorFromJson(componentJson, "origin");
            spriteManager.CopySpriteOrigin(origin, entity, 1);
            break;
        }
        case neko::NekoComponentType::SPINE_ANIMATION:
            break;
        case neko::NekoComponentType::BODY2D:
            break;
        case neko::NekoComponentType::COLLIDER2D:
            break;
        case neko::NekoComponentType::CONVEX_SHAPE2D:
            break;
        default:
            break;
    }
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
    auto& entityManager = nekoEditor_.GetEntityManager();
    entityManager.CreateEntity(entity);
    if (neko::CheckJsonParameter(entityJson, "name", json::value_t::string))
    {
        ResizeIfNecessary(currentScene_.entitiesNames, entity, std::string());
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


json EditorSceneManager::SerializeComponent(neko::Entity entity, neko::NekoComponentType componentType)
{
    json componentJson;
    componentJson["component"] = int(componentType);
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
        case neko::NekoComponentType::ANGLE2D:
        {
            auto& angleManager = nekoEditor_.GetAngleManager();
            componentJson["angle"] = angleManager.GetComponent(entity);
            break;
        }
        case neko::NekoComponentType::SPRITE2D:
        {
            auto& spriteManager = nekoEditor_.GetSpriteManager();
            auto& textureManager = nekoEditor_.GetTextureManager();
            const auto& sprite = spriteManager.GetComponent(entity);
            componentJson["texture"] = textureManager.GetTexturePath(sprite.textureId);
            componentJson["layer"] = sprite.layer;
            componentJson["origin"] = {sprite.origin.x, sprite.origin.y};
            break;
        }
        case neko::NekoComponentType::SPINE_ANIMATION:
            break;
        case neko::NekoComponentType::BODY2D:
            break;
        case neko::NekoComponentType::COLLIDER2D:
            break;
        case neko::NekoComponentType::CONVEX_SHAPE2D:
            break;
        default:
            break;
    }
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

    auto& entityManager = nekoEditor_.GetEntityManager();

    for (neko::Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.EntityExists(entity))
        {
            auto entityJson = SerializeEntity(entity);
            sceneJson["entities"].push_back(entityJson);
        }
    }
    return sceneJson;
}

}