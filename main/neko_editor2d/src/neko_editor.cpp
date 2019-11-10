
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
#include <tools/neko_editor.h>
#include <utilities/file_utility.h>
#include <engine/log.h>
#include "sfml_engine/graphics.h"

#include <SFML/Window/Event.hpp>
#include <sfml_engine/anim.h>
#include "tools/scene_system.h"
#include "tools/texture_editor.h"

namespace neko::editor
{

NekoEditor::NekoEditor() : SfmlBasicEngine(nullptr)
{
}

void NekoEditor::Init()
{
	SfmlBasicEngine::Init();
	//sceneRenderTexture_.create(config.gameWindowSize.first, config.gameWindowSize.second);
	neko::IterateDirectory(config.dataRootPath, [this](std::string_view path)
		{
			if (!neko::IsRegularFile(path))
				return;
			if (textureManager_.HasValidExtension(path))
			{
				textureManager_.LoadTexture(path.data());
			}
			if (neko::GetFilenameExtension(path) == ".prefab")
			{
				//TODO have own prefabmanager for all
				//prefabManager_.LoadPrefab(path, false);
			}
		}, true);
	const std::function<void(float)> editorUiFunc = [this](float dt)
	{
		EditorUpdate(dt);
	};
	drawUiDelegate_.RegisterCallback(editorUiFunc);
}

void NekoEditor::Destroy()
{
	SfmlBasicEngine::Destroy();
}


/*
void NekoEditor::SwitchEditorMode(EditorMode editorMode)
{
	if (editorMode == editorMode_)
	{
		return;
	}
	switch (editorMode)
	{
	case EditorMode::SceneMode:
	{
		if (editorMode_ == EditorMode::PrefabMode)
		{
			prefabManager_.SaveCurrentPrefab();
		}
		entityViewer_.Reset();

		sceneManager_.ClearScene();
		auto& scenePathName = sceneManager_.GetCurrentScene().scenePath;
		if (!scenePathName.empty())
			sceneManager_.LoadScene(scenePathName);
		editorMode_ = EditorMode::SceneMode;

		break;
	}
	case EditorMode::PrefabMode:
	{
		if (editorMode_ == EditorMode::SceneMode)
		{
			sceneManager_.SaveCurrentScene();
		}
		entityViewer_.Reset();
		sceneManager_.ClearScene();
		editorMode_ = EditorMode::PrefabMode;
		if (prefabManager_.GetCurrentPrefabIndex() != neko::INVALID_INDEX)
		{
			const auto prefabIndex = prefabManager_.LoadPrefab(prefabManager_.GetCurrentPrefabPath(), true);
			prefabManager_.InstantiatePrefab(prefabIndex, entityManager_);
		}
		else
		{
			const auto rootEntity = entityManager_.CreateEntity();
			auto& entitiesName = sceneManager_.GetCurrentScene().entitiesNames;
			ResizeIfNecessary(entitiesName, rootEntity, std::string());
			entitiesName[rootEntity] = "Root Entity";

		}
		break;
	}
	case EditorMode::TextureMode:
		break;
	case EditorMode::AnimMode:
		break;
	}
}
*/

void NekoEditor::OnEvent(sf::Event& event)
{
	SfmlBasicEngine::OnEvent(event);
	BasicEditorSystem* editorSystem = GetCurrentEditorSystem();

	if (event.type != sf::Event::KeyPressed)
		return;

	if (event.key.control)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::S:
		{
			Save(event.key.shift);
			break;
		}
		case sf::Keyboard::N:
		{
			CreateNewScene();
			break;
		}
		case sf::Keyboard::O:
		{
			OpenFileDialog();
			currentFileOperation_ = FileOperation::OPEN;
			break;
		}
		default:
		{
			break;
		}
		}
	}

}

/*
case sf::Keyboard::N:
{
	sceneManager_.ClearScene();
	const auto rootEntity = entityManager_.CreateEntity();
	auto& entitiesName = sceneManager_.GetCurrentScene().entitiesNames;
	ResizeIfNecessary(entitiesName, rootEntity, std::string());
	entitiesName[rootEntity] = "Root Entity";
	break;
}
case sf::Keyboard::O:
{
	fileOperationStatus_ = FileOperation::OPEN_PREFAB;
	break;
}
case sf::Keyboard::S:
{
	SavePrefabEvent();
	break;
}
	 */




	 /*
	 void NekoEditor::SaveSceneEvent()
	 {

		 auto& path = sceneManager_.GetCurrentScene().scenePath;
		 if (sceneManager_.IsCurrentSceneTmp() or !neko::FileExists(path))
		 {

			 fileOperationStatus_ = FileOperation::SAVE_SCENE;
		 }
		 else
		 {
			 sceneManager_.SaveCurrentScene();
		 }

	 }

	 void NekoEditor::SavePrefabEvent()
	 {
		 auto& path = prefabManager_.GetCurrentPrefabPath();
		 if (prefabManager_.IsCurrentPrefabTmp() or !neko::FileExists(path))
		 {
			 fileDialog_ = ImGui::FileBrowser(
				 ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
			 fileDialog_.SetPwd("../" + config.dataRootPath);
			 fileDialog_.Open();
			 fileOperationStatus_ = FileOperation::SAVE_PREFAB;
		 }
		 else
		 {
			 prefabManager_.SaveCurrentPrefab();
		 }

	 }
	 */

void NekoEditor::EditorUpdate([[maybe_unused]] float dt)
{
	const ImVec2 windowSize = ImVec2(float(config.realWindowSize.first), float(config.realWindowSize.second));
	const static float yOffset = 20.0f;
	ImGui::SetNextWindowPos(ImVec2(0.0f, windowSize.y * 0.7f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.8f, windowSize.y * 0.3f), ImGuiCond_Always);
	ImGui::Begin("Debug Window", nullptr,
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse);

	//Log tab
	if (ImGui::BeginTabBar("Lower Tab", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Debug Log"))
		{
			logViewer_.Update();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End(); //Debug Window

	ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.2f, yOffset), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.6f, windowSize.y * 0.7f - yOffset), ImGuiCond_Always);
	ImGui::Begin("Central Viewer", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "CTRL+N"))
			{
				CreateNewScene();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Open", "CTRL+O"))
			{
				OpenFileDialog();
				currentFileOperation_ = FileOperation::OPEN;
			}
			if (ImGui::MenuItem("Save", "CTRL+S"))
			{
				Save(false);
			}
			if (ImGui::MenuItem("Save As...", "CTRL+SHIFT+S"))
			{
				Save(true);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Windows"))
		{
			if (ImGui::MenuItem("Texture Window"))
			{
				const auto textureId = textureManager_.GetTextureNameMap().begin()->first;
				auto textureEditor = std::make_unique<TextureEditorSystem>(*this, textureManager_);
				textureEditor->Init();
				textureEditor->SetCurrentTextureId(textureId);
				SetCurrentEditorSystem(textureEditor.get());
				editorSystems_.push_back(std::move(textureEditor));
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (ImGui::BeginTabBar("Central Tab"))
	{
		for (Index i = 0; i < editorSystems_.size(); i++)
		{
			auto* editorSystem = editorSystems_[i].get();
			ImGui::PushID(editorSystem->GetEditorSystemId());
			//Show main view tabs
			if (ImGui::BeginTabItem(editorSystem->GetSystemName().c_str(), nullptr,
				editorSystem->GetEditorSystemId() == currentEditorSystemId_
				? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None))
			{
				editorSystem->OnMainView();
				ImGui::EndTabItem();
			}

			if (ImGui::IsItemClicked(0))
			{
				currentEditorSystemId_ = editorSystem->GetEditorSystemId();
			}
			ImGui::PopID();
		}

		ImGui::EndTabBar();
	}
	ImGui::End(); //Central viewer




	BasicEditorSystem* currentEditorSystem = GetCurrentEditorSystem();

	if (currentEditorSystem != nullptr)
	{
		currentEditorSystem->Update(dt);
	}


	ImGui::SetNextWindowPos(ImVec2(0.0f, yOffset), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.2f, windowSize.y * 0.7f - yOffset), ImGuiCond_Always);
	ImGui::Begin("Listing Viewer", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	if (currentEditorSystem != nullptr)
	{
		currentEditorSystem->OnListingView();
	}
	ImGui::End();//Listing
	ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.8f, yOffset), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.2f, windowSize.y - yOffset), ImGuiCond_Always);
	ImGui::Begin("Inspector", nullptr,
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	if (currentEditorSystem != nullptr)
	{
		currentEditorSystem->OnInspectorView();
	}
	ImGui::End(); //Inspector


	fileDialog_.Display();

	switch (currentFileOperation_)
	{
	case FileOperation::OPEN:
	{
		if (fileDialog_.HasSelected())
		{
			const auto assetPath = fileDialog_.GetSelected();

			OpenAsset(assetPath.string());
			fileDialog_.ClearSelected();
			fileDialog_.Close();

			currentFileOperation_ = FileOperation::NONE;
		}

		if (!fileDialog_.IsOpened())
		{
			logDebug("[Warning] Opening file operation was canceled");
			fileDialog_.ClearSelected();
			fileDialog_.Close();
		}
		break;
	}
	case FileOperation::SAVE:
		if (fileDialog_.HasSelected())
		{
			const auto assetPath = fileDialog_.GetSelected();
			SaveAsset(assetPath.string());
			fileDialog_.ClearSelected();
			fileDialog_.Close();

			currentFileOperation_ = FileOperation::NONE;
		}
		if (!fileDialog_.IsOpened())
		{
			logDebug("[Warning] Saving file operation was canceled");
			currentFileOperation_ = FileOperation::NONE;
			fileDialog_.ClearSelected();
			fileDialog_.Close();
		}
	default:
		break;
	}

	/*
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			switch (editorMode_)
			{
			case EditorMode::SceneMode:
			{

				break;
			}
			case EditorMode::PrefabMode:
			{
				if (ImGui::MenuItem("New Prefab", "CTRL+N"))
				{
					sceneManager_.ClearScene();
				}
				if (ImGui::MenuItem("Open Prefab", "CTRL+O"))
				{
					fileOperationStatus_ = FileOperation::OPEN_PREFAB;
				}

				if (ImGui::MenuItem("Save Prefab", "CTRL+S"))
				{
					SavePrefabEvent();
				}
				break;
			}
			}


			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	fileDialog_.Display();


	switch (fileOperationStatus_)
	{
	case FileOperation::OPEN_SCENE:
	{
		ImGui::OpenPopup("Scene Open Popup");
		fileOperationStatus_ = FileOperation::NONE;
		break;
	}
	case FileOperation::OPEN_PREFAB:
	{
		ImGui::OpenPopup("Prefab Open Popup");
		fileOperationStatus_ = FileOperation::NONE;
		break;
	}
	case FileOperation::SAVE_SCENE:
	{
		if (fileDialog_.HasSelected())
		{
			const auto sceneJsonPath = fileDialog_.GetSelected().string();
			sceneManager_.GetCurrentScene().scenePath = sceneJsonPath;
			sceneManager_.SaveCurrentScene();
			fileDialog_.ClearSelected();
			fileDialog_.Close();
		}
		break;
	}
	case FileOperation::SAVE_PREFAB:
	{
		if (fileDialog_.HasSelected())
		{
			const auto prefabJsonPath = fileDialog_.GetSelected().string();
			prefabManager_.SetCurrentPrefabPath(prefabJsonPath);
			prefabManager_.SaveCurrentPrefab();
			fileDialog_.ClearSelected();
			fileDialog_.Close();
		}
		break;
	}
	default:
		break;
	}

	if (ImGui::BeginPopup("Scene Open Popup"))
	{
		static std::vector<std::string> sceneFileList;
		if (sceneFileList.empty())
		{
			neko::IterateDirectory(neko::LinkFolderAndFile("..", config.dataRootPath),
				[](const std::string_view filename)
				{
					if (filename.find(".scene") != std::string_view::npos)
					{
						sceneFileList.push_back(filename.data());
					}
				}, true);
		}
		ImGui::Selectable("Cancel Open Scene...");
		for (const auto& sceneFilename : sceneFileList)
		{
			if (ImGui::Selectable(sceneFilename.c_str()))
			{
				sceneManager_.ClearScene();
				sceneManager_.LoadScene(sceneFilename);
				sceneFileList.clear();
			}

		}
		ImGui::EndPopup();
	}

	if (ImGui::BeginPopup("Prefab Open Popup"))
	{
		static std::vector<std::string> prefabFileList;
		if (prefabFileList.empty())
		{
			neko::IterateDirectory("../" + config.dataRootPath, [](const std::string_view filename)
				{
					if (filename.find(".prefab") != std::string_view::npos)
					{
						prefabFileList.push_back(filename.data());
					}
				}, true);
		}
		ImGui::Selectable("Cancel Open Prefab...");
		for (auto& prefabFilename : prefabFileList)
		{
			if (ImGui::Selectable(prefabFilename.c_str()))
			{
				sceneManager_.ClearScene();
				const auto prefabIndex = prefabManager_.LoadPrefab(prefabFilename, true);
				prefabManager_.InstantiatePrefab(prefabIndex, entityManager_);
				prefabFileList.clear();
			}

		}
		ImGui::EndPopup();
	}


	ImGui::End();
	ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.8f, windowSize.y * 0.7f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.2f, windowSize.y * 0.3f), ImGuiCond_Always);
	ImGui::Begin("Previewer", nullptr,

		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse);
	ImGui::End();


	//Render things into the graphics manager

	switch (editorMode_)
	{

	case EditorMode::PrefabMode:
	{
		auto rect = prefabManager_.CalculatePrefabBound();
		const auto screenRect = sf::FloatRect(sf::Vector2f(), sf::Vector2f(sceneRenderTexture_.getSize()));
		const auto rectRatio = rect.width / rect.height;
		const auto screenRatio = float(screenRect.width) / screenRect.height;
		rect.width *= screenRatio / rectRatio;
		const sf::View renderView(rect);
		sceneRenderTexture_.setView(renderView);
		RenderTarget renderTarget{ &sceneRenderTexture_ };
		graphicsManager_.RenderAll(&renderTarget);
		break;
	}
	case EditorMode::TextureMode:
		break;
	case EditorMode::AnimMode:
		break;
	}

*/


}

void NekoEditor::CreateNewScene()
{

	std::unique_ptr<EditorSceneSystem> newSceneSystem = std::make_unique<EditorSceneSystem>(*this,
		textureManager_);
	newSceneSystem->Init();
	SetCurrentEditorSystem(newSceneSystem.get());
	editorSystems_.push_back(std::move(newSceneSystem));

}


NekoEditorSystem::NekoEditorSystem(NekoEditor& nekoEditor, sfml::TextureManager& textureManager) :
	BasicEditorSystem(nekoEditor.config),
	editorExport_{
			{{entityManager_,
			  prefabManager_,
			  sceneManager_},
			 position2dManager_,
			 scale2dManager_,
			 rotation2dManager_,
			 transform2dManager_,
			 spriteManager_,
			 spineManager_,
			 spineBoneFollowerManager_
			},
			sceneManager_,
			bodyDef2DManager_,
			textureManager,
			boxColliderDefManager_,
			circleColliderDefManager_,
			polygonColldierDefManager_,
			colliderManagerDefManager_,
			prefabManager_,
			nekoEditor.config,
			nekoEditor,
			entityNameManager_
},
transform2dManager_(position2dManager_, scale2dManager_, rotation2dManager_),
sceneManager_(editorExport_),
spriteManager_(textureManager),
colliderManagerDefManager_(editorExport_),
prefabManager_(editorExport_),
entityViewer_(editorExport_),
inspector_(editorExport_),
entityNameManager_(entityManager_)
{
}

BasicEditorSystem::BasicEditorSystem(Configuration& config) : config_(config)
{

}

const std::string& BasicEditorSystem::GetResourcePath() const
{
	return resourcePath_;
}

void BasicEditorSystem::SetResourcePath(const std::string& resourcePath)
{
	resourcePath_ = resourcePath;
}

bool BasicEditorSystem::IsTmpResource() const
{
	return resourcePath_.empty();
}

EditorSystemMode NekoEditor::GetEditorSystemModeFrom(const std::string_view extension)
{
	if (extension == SceneManager::GetExtension())
	{
		return EditorSystemMode::SceneMode;
	}
	if (extension == PrefabManager::GetExtension())
	{
		return EditorSystemMode::PrefabMode;
	}
	if (extension == sfml::TextureManager::GetMetaExtension() or
		sfml::TextureManager::HasValidExtension(extension))
	{
		return EditorSystemMode::TextureMode;
	}
	if (extension == sfml::AnimatorManager::GetExtension())
	{
		return EditorSystemMode::AnimMode;
	}
	return EditorSystemMode::None;
}

void NekoEditor::OpenFileDialog()
{
	fileDialog_ = ImGui::FileBrowser(
		ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
	fileDialog_.SetPwd("../" + config.dataRootPath);
	fileDialog_.Open();
}

void NekoEditor::OpenAsset(const std::string_view assetPath)
{
	const auto extension = GetFilenameExtension(assetPath);
	const auto editorMode = GetEditorSystemModeFrom(extension);

	switch (editorMode)
	{
	case EditorSystemMode::SceneMode:
	{
		//Check if scene is already loaded
		json sceneJson = LoadJson(assetPath);
		if (CheckJsonParameter(sceneJson, "sceneId", json::value_t::string))
		{
			const SceneId sceneId = sole::rebuild(sceneJson["sceneId"]);
			const EditorSystemId editorSystemId = EditorSceneSystem::GenerateEditorSystemIdFrom(sceneId);
			const auto editorSystemIt = editorSystemMap_.find(editorSystemId);
			if (editorSystemIt != editorSystemMap_.end())
			{
				currentEditorSystemId_ = editorSystemId;
				currentEditorMode_ = editorSystemIt->second->GetEditorMode();
				return;
			}
		}
		//Open scene in new scene system
		auto newSceneSystem = std::make_unique<EditorSceneSystem>(*this, textureManager_);
		newSceneSystem->Init();

		newSceneSystem->OpenScene(assetPath);
		SetCurrentEditorSystem(newSceneSystem.get());
		editorSystems_.push_back(std::move(newSceneSystem));
		break;
	}
	case EditorSystemMode::TextureMode:
	{
		std::string texturePath = assetPath.data();
		if (texturePath.find(sfml::TextureManager::GetMetaExtension()) != std::string::npos)
		{
			texturePath = GetFileParentPath(texturePath) + GetStem(texturePath);
		}
		const auto textureId = textureManager_.LoadTexture(texturePath);
		//Check if texture is already open in another tab
		{
			auto editorSystem = std::find_if(editorSystems_.begin(), editorSystems_.end(),
				[](const std::unique_ptr<BasicEditorSystem>& editorSystem)
				{
					return editorSystem->GetEditorMode() == EditorSystemMode::TextureMode;
				});
			if (editorSystem != editorSystems_.end())
			{
				RemoveEditorSystem(editorSystem->get()->GetEditorSystemId(), false);
				auto textureEditor = std::make_unique<TextureEditorSystem>(*this, textureManager_);
				textureEditor->Init();
				textureEditor->SetCurrentTextureId(textureId);
				SetCurrentEditorSystem(textureEditor.get());
				*editorSystem = std::move(textureEditor);
			}
		}
		{
			const auto editorSystem = editorSystemMap_.find(textureId);
			if (editorSystem != editorSystemMap_.end())
			{
				currentEditorMode_ = editorSystem->second->GetEditorMode();
				currentEditorSystemId_ = textureId;
				return;
			}
		}


		auto textureEditor = std::make_unique<TextureEditorSystem>(*this, textureManager_);
		textureEditor->Init();
		textureEditor->SetCurrentTextureId(textureId);
		SetCurrentEditorSystem(textureEditor.get());
		editorSystems_.push_back(std::move(textureEditor));
		break;
	}
	default:
	{
		std::ostringstream oss;
		oss << "[Warning] Missing case for opening resource: " << Index(editorMode);
		logDebug(oss.str());
		break;
	}
	}
}

void NekoEditor::RemoveEditorSystem(EditorSystemId editorSystemId, bool destroy)
{
	if (editorSystemId == INVALID_EDITOR_SYSTEM_ID)
	{
		return;
	}
	if (destroy)
	{
		auto* editorSystemPtr = editorSystemMap_[editorSystemId];
		const auto editorSystem = std::find_if(editorSystems_.begin(), editorSystems_.end(),
			[editorSystemPtr](const std::unique_ptr<BasicEditorSystem>& editorSystem)
			{
				return editorSystem.get() == editorSystemPtr;
			});
		editorSystems_.erase(editorSystem);
	}
	editorSystemMap_.erase(editorSystemId);
}

void NekoEditor::SetCurrentEditorSystem(BasicEditorSystem* editorSystem)
{
	currentEditorMode_ = editorSystem->GetEditorMode();
	currentEditorSystemId_ = editorSystem->GetEditorSystemId();
	editorSystemMap_[currentEditorSystemId_] = editorSystem;
}

BasicEditorSystem* NekoEditor::GetCurrentEditorSystem()
{
	BasicEditorSystem* currentEditorSystem = nullptr;
	if (currentEditorSystemId_ != INVALID_EDITOR_SYSTEM_ID)
	{
		currentEditorSystem = editorSystemMap_[currentEditorSystemId_];
	}
	return currentEditorSystem;
}

void NekoEditor::SaveAsset(const std::string_view assetPath)
{
	auto* currentEditorSystem = GetCurrentEditorSystem();
	switch (currentEditorMode_)
	{
	case EditorSystemMode::SceneMode:
	{
		if (currentEditorSystem->IsTmpResource() or currentEditorSystem->GetResourcePath() != assetPath)
		{
			if (assetPath.find(SceneManager::GetExtension()) == std::string::npos)
			{
				std::string newResourcePath = assetPath.data();
				newResourcePath += SceneManager::GetExtension().data();
				currentEditorSystem->SetResourcePath(newResourcePath);

			}
			else
			{
				currentEditorSystem->SetResourcePath(assetPath.data());
			}
			auto stem = GetStem(assetPath);
			currentEditorSystem->SetSystemName(stem);
		}

		currentEditorSystem->OnSave();
		break;
	}
	case EditorSystemMode::TextureMode:
	{
		currentEditorSystem->OnSave();
		break;
	}
	default:
	{
		std::ostringstream oss;
		oss << "[Warning] Missing case for saving resource: " << Index(currentEditorMode_);
		logDebug(oss.str());
		break;
	}
	}
}

void NekoEditor::Save(bool saveAs)
{
	auto* currentEditorSystem = GetCurrentEditorSystem();
	if (currentEditorSystem != nullptr)
	{
		if (saveAs or currentEditorSystem->IsTmpResource())
		{
			OpenFileDialog();
			currentFileOperation_ = FileOperation::SAVE;
		}
		else
		{
			currentEditorSystem->OnSave();
		}

	}
	else
	{
		logDebug("[Warning] Saving editor system, but there is none...");
	}
}


}
