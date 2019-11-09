
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
    BasicEditorSystem* editorSystem = nullptr;
    if (currentSystemIndex < editorSystems_.size())
    {
        editorSystem = editorSystems_[currentSystemIndex].get();
        editorSystem->OnEvent(event);
    }

    if (event.type != sf::Event::KeyPressed)
        return;

    if (event.key.control)
    {
        switch (event.key.code)
        {
            case sf::Keyboard::S:
            {
                if (editorSystem != nullptr)
                {
                    editorSystem->OnSave();
                }
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
                break;
            }
            default:
            {
                break;
            }
        }
    }

    switch (editorMode_)
    {
        case EditorSystemMode::SceneMode:
        {
            if (event.key.control)
            {
                switch (event.key.code)
                {

                    default:
                        break;
                }
            }
            break;
        }
        case EditorSystemMode::PrefabMode:
            if (event.key.control)
            {
                switch (event.key.code)
                {
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
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }

}

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

void NekoEditor::EditorUpdate([[maybe_unused]]float dt)
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
            }
            if (ImGui::MenuItem("Save", "CTRL+S"))
            {
                //TODO save the scene
                //SaveSceneEvent();
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
            //Show main view tabs
            if (ImGui::BeginTabItem(editorSystem->GetSystemName().c_str(), nullptr,
                                    currentSystemIndex == i ? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None))
            {
                editorSystem->OnMainView();
                ImGui::EndTabItem();
            }
            if (ImGui::IsItemClicked(0))
            {
                if (currentSystemIndex < editorSystems_.size())
                {
                    editorSystems_[i].get()->OnLostFocus();
                }
                currentSystemIndex = i;
            }
        }

        ImGui::EndTabBar();
    }
    ImGui::End(); //Central viewer




    BasicEditorSystem* currentEditorSystem = nullptr;
    //TODO Set the selected system depending on the index
    if (currentSystemIndex < editorSystems_.size())
    {
        currentEditorSystem = editorSystems_[currentSystemIndex].get();

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
                const auto extension = GetFilenameExtension(assetPath.string());
                const auto editorMode = GetEditorSystemModeFrom(extension);

                switch (editorMode)
                {
                    case EditorSystemMode::SceneMode:
                    {
                        //TODO Check if scene is already loaded

                        //Open scene in new scene system
                        auto newSceneSystem = std::make_unique<EditorSceneSystem>(*this, textureManager_);
                        newSceneSystem->Init();

                        newSceneSystem->OpenScene(assetPath.c_str());
                        editorMode_ = newSceneSystem->GetEditorMode();

                        editorSystems_.push_back(std::move(newSceneSystem));
                        currentSystemIndex = editorSystems_.size() - 1;
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
                fileDialog_.ClearSelected();
                fileDialog_.Close();
            }
            break;
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
    case EditorMode::SceneMode:
    {

        break;
    }
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
    newSceneSystem->SetSceneId(SceneManager::GenerateSceneId());
    newSceneSystem->Init();
    editorSystems_.push_back(std::move(newSceneSystem));
    currentSystemIndex = editorSystems_.size() - 1;
    editorMode_ = editorSystems_[currentSystemIndex]->GetEditorMode();

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
                 spineManager_},
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
        }, transform2dManager_(position2dManager_, scale2dManager_, rotation2dManager_),
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

EditorSystemMode NekoEditor::GetEditorSystemModeFrom(const std::string_view extension)
{
    if (extension == SceneManager::GetExtension())
    {
        return EditorSystemMode::SceneMode;
    }
    else if (extension == PrefabManager::GetExtension())
    {
        return EditorSystemMode::PrefabMode;
    }
    else if (extension == sfml::TextureManager::GetMetaExtension() or
             sfml::TextureManager::HasValidExtension(extension))
    {
        return EditorSystemMode::TextureMode;
    }
    else if (extension == sfml::AnimatorManager::GetExtension())
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
    currentFileOperation_ = FileOperation::OPEN;
}


}
