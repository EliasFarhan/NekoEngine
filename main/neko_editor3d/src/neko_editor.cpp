
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
#include <tools/prefab_system.h>
#include "tools/scene_system.h"
#include "tools/texture_editor.h"
#include "tools/spine_editor.h"
#include "sfml_engine/texture.h"

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
        if (HasValidExtension<sfml::Texture>(path))
        {
            textureManager_.LoadTexture(path.data());
        }
        if (neko::GetFilenameExtension(path) == PrefabManager::GetExtension())
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
        if (ImGui::BeginMenu("Assets"))
        {
            if (ImGui::MenuItem("New Prefab"))
            {
                auto prefabSystem = std::make_unique<EditorPrefabSystem>(*this, textureManager_);
                prefabSystem->Init();
                SetCurrentEditorSystem(prefabSystem.get());
                editorSystems_.push_back(std::move(prefabSystem));

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
        for (auto& basicEditorSystem : editorSystems_)
        {
            auto* editorSystem = basicEditorSystem.get();
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
                currentFileOperation_ = FileOperation::NONE;
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
                {
                        {
                                entityManager_,
                                prefabManager_,
                                sceneManager_
                        },
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
        }, position2dManager_(entityManager_), rotation2dManager_(entityManager_), scale2dManager_(entityManager_),
        transform2dManager_(position2dManager_, scale2dManager_, rotation2dManager_),
        sceneManager_(editorExport_), bodyDef2DManager_(entityManager_),
        spriteManager_(entityManager_, textureManager),
        spineManager_(entityManager_),
        spineBoneFollowerManager_(entityManager_),
        boxColliderDefManager_(entityManager_),
        circleColliderDefManager_(entityManager_),
        polygonColldierDefManager_(entityManager_),
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
        HasValidExtension<sfml::Texture>(extension))
    {
        return EditorSystemMode::TextureMode;
    }
    if (extension == sfml::AnimatorManager::GetExtension())
    {
        return EditorSystemMode::AnimMode;
    }
    if(extension == sfml::SpineManager::GetExtension())
    {
        return EditorSystemMode::SpineMode;
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
                    SetCurrentEditorSystem(editorSystemIt->second);
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
        case EditorSystemMode::PrefabMode:
        {
            //Check if there is no other prefab scene system open
            json prefabJson = LoadJson(assetPath);
            if (CheckJsonParameter(prefabJson, "prefabId", json::value_t::string))
            {
                const PrefabId prefabId = sole::rebuild(prefabJson["prefabId"]);
                const EditorSystemId editorSystemId = EditorSceneSystem::GenerateEditorSystemIdFrom(prefabId);
                const auto editorSystemIt = editorSystemMap_.find(editorSystemId);
                if (editorSystemIt != editorSystemMap_.end())
                {
                    SetCurrentEditorSystem(editorSystemIt->second);
                    return;
                }
            }
            //Open prefab scene in a new prefab system
            auto newPrefabSystem = std::make_unique<EditorPrefabSystem>(*this, textureManager_);
            std::string resourcePath = MakeGeneric(assetPath);
            newPrefabSystem->SetResourcePath(resourcePath);
            newPrefabSystem->Init();
            SetCurrentEditorSystem(newPrefabSystem.get());
            editorSystems_.push_back(std::move(newPrefabSystem));
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
            //Check if texture previewer is already open in another tab
            {
                const auto editorSystem = std::find_if(editorSystems_.begin(), editorSystems_.end(),
                                                       [](const std::unique_ptr<BasicEditorSystem>& editorSystem)
                                                       {
                                                           return editorSystem->GetEditorMode() ==
                                                                  EditorSystemMode::TextureMode;
                                                       });
                if (editorSystem != editorSystems_.end())
                {
                    RemoveEditorSystem(editorSystem->get()->GetEditorSystemId(), false);
                    auto textureEditor = static_cast<TextureEditorSystem*>(editorSystem->get());
                    textureEditor->Init();
                    textureEditor->SetCurrentTextureId(textureId);
                    SetCurrentEditorSystem(textureEditor);
                    break;
                }
            }


            auto textureEditor = std::make_unique<TextureEditorSystem>(*this, textureManager_);
            textureEditor->Init();
            textureEditor->SetCurrentTextureId(textureId);
            SetCurrentEditorSystem(textureEditor.get());
            editorSystems_.push_back(std::move(textureEditor));
            break;
        }
        case EditorSystemMode::SpineMode:
        {
            std::string spinePath = assetPath.data();
            //Check if path is already open
            {
                const auto editorSystem = std::find_if(editorSystems_.begin(), editorSystems_.end(),
                                                       [](const std::unique_ptr<BasicEditorSystem>& editorSystem)
                                                       {
                                                           return editorSystem->GetEditorMode() ==
                                                                  EditorSystemMode::SpineMode;
                                                       });
                if(editorSystem != editorSystems_.end())
                {
                    RemoveEditorSystem(editorSystem->get()->GetEditorSystemId(), false);
                    auto spineEditor = static_cast<SpineEditorSystem*>(editorSystem->get());
                    spineEditor->Init();
                    spineEditor->SetCurrentSpine(spinePath);
                    SetCurrentEditorSystem(spineEditor);
                    break;
                }
            }
            auto spineEditor = std::make_unique<SpineEditorSystem>(config);
            spineEditor->Init();
            spineEditor->SetCurrentSpine(spinePath);
            SetCurrentEditorSystem(spineEditor.get());
            editorSystems_.push_back(std::move(spineEditor));

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
    const auto updateEditorName = [this, currentEditorSystem, &assetPath](std::string_view extension)
    {
        if (currentEditorSystem->IsTmpResource() or currentEditorSystem->GetResourcePath() != assetPath)
        {
            std::string newResourcePath = assetPath.data();
            if (assetPath.find(extension) == std::string::npos)
            {
                newResourcePath += extension.data();
            }
            newResourcePath = MakeGeneric(newResourcePath);
            newResourcePath =
                    "../" + config.dataRootPath + GetRelativePath(newResourcePath, "../" + config.dataRootPath);
            currentEditorSystem->SetResourcePath(newResourcePath);
            const auto stem = GetStem(assetPath);
            currentEditorSystem->SetSystemName(stem);
        }
    };
    switch (currentEditorMode_)
    {
        case EditorSystemMode::SceneMode:
        {
            updateEditorName(SceneManager::GetExtension());
            currentEditorSystem->OnSave();
            break;
        }

        case EditorSystemMode::PrefabMode:
        {
            updateEditorName(PrefabManager::GetExtension());
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
