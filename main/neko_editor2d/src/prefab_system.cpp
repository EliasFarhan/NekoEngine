
#include "tools/prefab_system.h"
#include <tools/neko_editor.h>

namespace neko::editor
{

void EditorPrefabSystem::Init()
{
    if (!resourcePath_.empty())
    {
        const auto prefabId = prefabManager_.LoadPrefab(resourcePath_);
        const auto& prefab = prefabManager_.GetPrefab(neko::PrefabId());
        sceneManager_.ParseSceneJson(prefab.prefabJson);

        currentPrefab_ = prefabManager_.GetPrefab(prefabId);
    }
    else
    {
        entityManager_.CreateEntity();
        entityNameManager_.SetComponent(0, "Root Entity");

    }
}

void EditorPrefabSystem::Update(float dt)
{
    auto rect = prefabManager_.CalculatePrefabBound();
    const auto screenRect = sf::FloatRect(sf::Vector2f(), sf::Vector2f(screenRenderTexture_.getSize()));
    const auto rectRatio = rect.width / rect.height;
    const auto screenRatio = float(screenRect.width) / screenRect.height;
    rect.width *= screenRatio / rectRatio;
    const sf::View renderView(rect);
    screenRenderTexture_.setView(renderView);
    RenderTarget renderTarget{&screenRenderTexture_};
    graphicsManager_.RenderAll(&renderTarget);
}

void EditorPrefabSystem::Destroy()
{

}

void EditorPrefabSystem::OnListingView()
{
    entityViewer_.Update(EditorSystemMode::PrefabMode);
}

void EditorPrefabSystem::OnMainView()
{
    sceneViewer_.Update(screenRenderTexture_);
}

void EditorPrefabSystem::OnInspectorView()
{
    if (entityViewer_.GetSelectedEntity() != INVALID_ENTITY)
    {
        inspector_.ShowEntityInfo(entityViewer_.GetSelectedEntity());
    }
}

void EditorPrefabSystem::OnSave()
{

}

EditorSystemId EditorPrefabSystem::GetEditorSystemId() const
{
    xxh::hash_state64_t prefabIdHashStream(0);
    prefabIdHashStream.update(currentPrefab_.id.str());
    return prefabIdHashStream.digest();
}
}