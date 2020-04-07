
#include "tools/prefab_system.h"
#include <tools/neko_editor.h>

namespace neko::editor
{

void EditorPrefabSystem::Init()
{
	editorMode_ = EditorSystemMode::PrefabMode;
	if (!resourcePath_.empty())
	{
		const auto prefabId = prefabManager_.LoadPrefab(resourcePath_);
		const auto& prefab = prefabManager_.GetPrefab(prefabId);
		sceneManager_.ParseSceneJson(prefab.prefabJson);
		prefabManager_.SetCurrentPrefab(prefab);
	}
	else
	{
		const Prefab prefab;
		prefabManager_.SetCurrentPrefab(prefab);
		entityManager_.CreateEntity();
		entityNameManager_.SetComponent(0, "Root Entity");
	}
	screenRenderTexture_.create(config_.gameWindowSize.first, config_.gameWindowSize.second);
}

void EditorPrefabSystem::Update(float dt)
{
	const auto& currentScene = sceneManager_.GetCurrentScene();
	screenRenderTexture_.clear(sf::Color(currentScene.bgColor));
	transform2dManager_.CopyAllFromSpineFollower(entityManager_, spineBoneFollowerManager_);
	spriteManager_.CopyAllTransforms(entityManager_, transform2dManager_);
	spriteManager_.PushAllCommands(entityManager_, graphicsManager_);

	spineManager_.Update(entityManager_, 0.0f);
	spineManager_.CopyAllTransforms(entityManager_, transform2dManager_);
	spineManager_.PushAllCommands(entityManager_, graphicsManager_);

	colliderManagerDefManager_.PushAllCommands(graphicsManager_);

	auto rect = prefabManager_.CalculatePrefabBound();

	const auto screenRect = sf::FloatRect(sf::Vector2f(), sf::Vector2f(screenRenderTexture_.getSize()));
	const auto rectRatio = rect.width / rect.height;
	const auto screenRatio = float(screenRect.width) / screenRect.height;
	rect.width *= screenRatio / rectRatio;
	const sf::View renderView(rect);
	screenRenderTexture_.setView(renderView);
	RenderTarget renderTarget{ &screenRenderTexture_ };
	graphicsManager_.RenderAll(&renderTarget);
	screenRenderTexture_.display();
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
	prefabManager_.SaveCurrentPrefab();
	auto& currentPrefab = prefabManager_.GetCurrentPrefab();
	prefabManager_.LoadPrefab(currentPrefab.prefabPath);
}

void EditorPrefabSystem::SetResourcePath(const std::string& resourcePath)
{
	BasicEditorSystem::SetResourcePath(resourcePath);
	if (!prefabManager_.IsCurrentPrefabValid())
		return;
	auto currentPrefab = prefabManager_.GetCurrentPrefab();
	currentPrefab.prefabPath = resourcePath_;
	prefabManager_.SetCurrentPrefab(currentPrefab);

}

EditorSystemId EditorPrefabSystem::GetEditorSystemId() const
{
	auto& currentPrefab = prefabManager_.GetCurrentPrefab();
	xxh::hash_state64_t prefabIdHashStream(0);
	prefabIdHashStream.update(currentPrefab.id.str());
	return prefabIdHashStream.digest();
}
}