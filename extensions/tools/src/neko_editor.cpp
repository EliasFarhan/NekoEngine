
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
#include <SFML/Window/Event.hpp>

namespace editor
{
void NekoEditor::Init()
{
    BasicEngine::Init();
    sceneRenderTexture_.create(config.gameWindowSize.x, config.gameWindowSize.y);
    neko::IterateDirectory(config.dataRootPath, [this](std::string_view path)
    {
        if (!neko::IsRegularFile(path))
            return;
        if (textureManager_.HasValidExtension(path))
        {
            textureManager_.LoadTexture(path);
        }
        if (neko::GetFilenameExtension(path) == ".prefab")
        {
            prefabManager_.LoadPrefab(path, false);
        }
    }, true);

}

void NekoEditor::Update(float dt)
{
    BasicEngine::Update(dt);
    const ImVec2 windowSize = ImVec2(float(config.realWindowSize.x), float(config.realWindowSize.y));
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
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            switch (editorMode_)
            {
                case EditorMode::SceneMode:
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
                        SaveSceneEvent();
                    }
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
                auto sceneJsonPath = fileDialog_.GetSelected().string();
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
            neko::IterateDirectory(neko::LinkFolderAndFile("..", config.dataRootPath), [](const std::string_view filename)
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
    previewer_.Update(dt);
    ImGui::End();


    //Draw things into the graphics manager
    sceneRenderTexture_.clear(config.bgColor);

    spriteManager_.CopyAllTransforms(entityManager_, transformManager_);
    spriteManager_.PushAllCommands(entityManager_, graphicsManager_);

    spineManager_.Update(entityManager_, dt);
    spineManager_.CopyAllTransforms(entityManager_, transformManager_);
    spineManager_.PushAllCommands(entityManager_, graphicsManager_);

    colliderDefManager_.PushAllCommands(graphicsManager_);
    switch (editorMode_)
    {
        case EditorMode::SceneMode:
        {
            const auto screenRect = sf::FloatRect(0, 0, float(config.gameWindowSize.x), float(config.gameWindowSize.y));
            sceneRenderTexture_.setView(sf::View(screenRect));
            graphicsManager_.Render(sceneRenderTexture_);
            break;
        }
        case EditorMode::PrefabMode:
        {
            auto rect = prefabManager_.CalculatePrefabBound();
            const auto screenRect = sf::FloatRect(sf::Vector2f(), sf::Vector2f(sceneRenderTexture_.getSize()));
            const auto rectRatio = rect.width / rect.height;
            const auto screenRatio = float(screenRect.width) / screenRect.height;
            rect.width *= screenRatio / rectRatio;
            sf::View renderView(rect);
            sceneRenderTexture_.setView(renderView);
            graphicsManager_.Render(sceneRenderTexture_);
            break;
        }
    }

    sceneRenderTexture_.display();

    ImGui::SetNextWindowPos(ImVec2(0.0f, yOffset), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.2f, windowSize.y * 0.7f - yOffset), ImGuiCond_Always);
    entityViewer_.Update(editorMode_);

    ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.2f, yOffset), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.6f, windowSize.y * 0.7f - yOffset), ImGuiCond_Always);
    ImGui::Begin("Central Viewer", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (ImGui::BeginTabBar("Central Tab"))
    {
        bool sceneViewerOpen = editorMode_ == EditorMode::SceneMode;
        if (ImGui::BeginTabItem("Scene Viewer", nullptr,
                                sceneViewerOpen ? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None))
        {
            sceneViewer_.Update(sceneRenderTexture_);
            ImGui::EndTabItem();
        }
        if (ImGui::IsItemClicked(0))
        {
            SwitchEditorMode(EditorMode::SceneMode);
        }
        bool prefabViewerOpen = editorMode_ == EditorMode::PrefabMode;
        if (ImGui::BeginTabItem("Prefab Viewer", nullptr,
                                prefabViewerOpen ? ImGuiTabItemFlags_SetSelected : ImGuiTabItemFlags_None))
        {
            sceneViewer_.Update(sceneRenderTexture_);
            ImGui::EndTabItem();
        }
        if (ImGui::IsItemClicked(0))
        {
            SwitchEditorMode(EditorMode::PrefabMode);
        }
        ImGui::EndTabBar();
    }

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.8f, yOffset), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.2f, windowSize.y * 0.7f - yOffset), ImGuiCond_Always);
    inspector_.BeginWindow();

    inspector_.ShowEntityInfo(entityViewer_.GetSelectedEntity());
    inspector_.EndWindow();
}

void NekoEditor::Destroy()
{
    BasicEngine::Destroy();
}

neko::BodyDef2dManager& NekoEditor::GetBodyDefManager()
{
    return bodyDefManager_;
}

neko::EntityManager& NekoEditor::GetEntityManager()
{
    return entityManager_;
}

neko::Position2dManager& NekoEditor::GetPositionManager()
{
    return transformManager_.GetPositionManager();
}

neko::Scale2dManager& NekoEditor::GetScaleManager()
{
    return transformManager_.GetScaleManager();
}

neko::Rotation2dManager& NekoEditor::GetRotationManager()
{
    return transformManager_.GetRotationManager();
}

EditorSceneManager& NekoEditor::GetSceneManager()
{
    return sceneManager_;
}

neko::SpriteManager& NekoEditor::GetSpriteManager()
{
    return spriteManager_;
}

neko::TextureManager& NekoEditor::GetTextureManager()
{
    return textureManager_;
}

Previewer& NekoEditor::GetPreviewer()
{
    return previewer_;
}

neko::SpineManager& NekoEditor::GetSpineManager()
{
    return spineManager_;
}

neko::Transform2dManager& NekoEditor::GetTransformManager()
{
    return transformManager_;
}

NekoEditor::NekoEditor()
        : BasicEngine(),
          entityViewer_(*this),
          prefabManager_(*this),
          spriteManager_(textureManager_),
          sceneManager_(*this),
          colliderDefManager_(*this),
          inspector_(*this)
{

}

ColliderDefManager& NekoEditor::GetColliderDefManager()
{
    return colliderDefManager_;
}

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
    }
}

EditorPrefabManager& NekoEditor::GetPrefabManager()
{
    return prefabManager_;
}

void NekoEditor::OnEvent(sf::Event& event)
{
    BasicEngine::OnEvent(event);
    if (event.type != sf::Event::KeyPressed)
        return;
    switch (editorMode_)
    {
        case EditorMode::SceneMode:
        {
            if (event.key.control)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::N:
                    {
                        sceneManager_.ClearScene();
                        sceneManager_.GetCurrentScene().scenePath = "";
                        break;
                    }
                    case sf::Keyboard::O:
                    {
                        fileOperationStatus_ = FileOperation::OPEN_SCENE;
                        break;
                    }
                    case sf::Keyboard::S:
                    {
                        SaveSceneEvent();
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        case EditorMode::PrefabMode:
            if (event.key.control)
            {
                switch (event.key.code)
                {
                    case sf::Keyboard::N:
                    {
                        sceneManager_.ClearScene();
                        auto rootEntity = entityManager_.CreateEntity();
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
                    default:
                        break;
                }
            }
            break;
        default:
            break;
    }
}

void NekoEditor::SaveSceneEvent()
{
    auto& path = sceneManager_.GetCurrentScene().scenePath;
    if (sceneManager_.IsCurrentSceneTmp() or !neko::FileExists(path))
    {
        fileDialog_ = ImGui::FileBrowser(
                ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
        fileDialog_.SetPwd("../" + config.dataRootPath);
        fileDialog_.Open();
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

}