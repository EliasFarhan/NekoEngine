#include <tools/neko_editor.h>
#include "tools/scene_system.h"

namespace neko::editor
{
void EditorSceneSystem::Init()
{

}

void EditorSceneSystem::Update(float dt)
{

	const auto& currentScene = sceneManager_.GetCurrentScene();
	screenRenderTexture_.clear(sf::Color(currentScene.bgColor));

	spriteManager_.CopyAllTransforms(entityManager_, transform2dManager_);
	spriteManager_.PushAllCommands(entityManager_, graphicsManager_);

	spineManager_.Update(entityManager_, 0.0f);
	spineManager_.CopyAllTransforms(entityManager_, transform2dManager_);
	spineManager_.PushAllCommands(entityManager_, graphicsManager_);

	colliderManagerDefManager_.PushAllCommands(graphicsManager_);
//TODO Have own Camera/View system per scene
	/*const auto screenRect = sf::FloatRect(
		0,
		0,
		float(config.gameWindowSize.first),
		float(config.gameWindowSize.second));
	screenRenderTexture_.setView(sf::View(screenRect));
	 */
	RenderTarget renderTarget{ &screenRenderTexture_ };
	graphicsManager_.RenderAll(&renderTarget);
	screenRenderTexture_.display();

}

void EditorSceneSystem::Destroy()
{

}

void EditorSceneSystem::SetSceneId(SceneId sceneId)
{
	auto scene = sceneManager_.GetCurrentScene();
	scene.sceneId = sceneId;
	sceneManager_.SetCurrentScene(scene);
}

void EditorSceneSystem::OnListingView()
{
	entityViewer_.Update(EditorMode::SceneMode);
}

void EditorSceneSystem::OnMainView()
{
	sceneViewer_.Update(screenRenderTexture_);
}

void EditorSceneSystem::OnInspectorView()
{
	if(entityViewer_.GetSelectedEntity() != INVALID_ENTITY)
	{
		inspector_.ShowEntityInfo(entityViewer_.GetSelectedEntity());
	}
}
}
