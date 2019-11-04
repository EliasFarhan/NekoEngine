#include <tools/neko_editor.h>
#include "tools/scene_system.h"

namespace neko::editor
{
void EditorSceneSystem::Init()
{

}

void EditorSceneSystem::Update(float dt)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "CTRL+N"))
			{
				sceneManager_.ClearScene();
			}
			if (ImGui::MenuItem("Open Scene", "CTRL+O"))
			{
				fileOperationStatus_ = FileOperation::OPEN_SCENE;
			}
			if (ImGui::MenuItem("Save Scene", "CTRL+S"))
			{
				//TODO save the scene
				//SaveSceneEvent();
			}
			ImGui::EndMenu();
		}
	}
	const auto& currentScene = sceneManager_.GetCurrentScene();
	screenRenderTexture_.clear(sf::Color(currentScene.bgColor));

	spriteManager_.CopyAllTransforms(entityManager_, transform2dManager_);
	spriteManager_.PushAllCommands(entityManager_, graphicsManager_);

	spineManager_.Update(entityManager_, 0.0f);
	spineManager_.CopyAllTransforms(entityManager_, transform2dManager_);
	spineManager_.PushAllCommands(entityManager_, graphicsManager_);

	colliderManagerDefManager_.PushAllCommands(graphicsManager_);

	const auto screenRect = sf::FloatRect(
		0,
		0,
		float(config.gameWindowSize.first),
		float(config.gameWindowSize.second));
	screenRenderTexture_.setView(sf::View(screenRect));
	RenderTarget renderTarget{ &screenRenderTexture_ };
	graphicsManager_.RenderAll(&renderTarget);
	screenRenderTexture_.display();
	
	entityViewer.Update(EditorMode::SceneMode);
	
	ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.8f, yOffset), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.2f, windowSize.y * 0.7f - yOffset), ImGuiCond_Always);
	inspector.BeginWindow();

	inspector.ShowEntityInfo(entityViewer.GetSelectedEntity());
	inspector.EndWindow();
}

void EditorSceneSystem::Destroy()
{

}

}