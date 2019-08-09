
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

namespace editor
{
void NekoEditor::Init()
{
    BasicEngine::Init();
    sceneRenderTexture_.create(config.gameWindowSize.x, config.gameWindowSize.y);
    neko::IterateDirectory(config.dataRootPath, [this](std::string_view path)
    {
        if (!neko::IsRegularFile(path) || !textureManager_.HasValidExtension(path))
            return;
        textureManager_.LoadTexture(path);
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
    static ImGui::FileBrowser fileDialog;
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "CTRL+N"))
            {
                sceneManager_.ClearScene();
            }
            if (ImGui::MenuItem("Open", "CTRL+O"))
            {
                fileDialog = ImGui::FileBrowser();
                fileDialog.SetPwd("../" + config.dataRootPath);
                fileDialog.Open();
                fileOperationStatus_ = FileOperation::OPEN;
            }
            if (ImGui::MenuItem("Save", "CTRL+S"))
            {
                auto& path = sceneManager_.GetCurrentScene().scenePath;
                if (path.empty() || !neko::FileExists(path))
                {
                    fileDialog = ImGui::FileBrowser(
                            ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
                    fileDialog.SetPwd("../" + config.dataRootPath);
                    fileDialog.Open();
                    fileOperationStatus_ = FileOperation::SAVE;
                }
                else
                {
                    sceneManager_.SaveScene(path);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    fileDialog.Display();


    if (fileDialog.HasSelected())
    {
        switch (fileOperationStatus_)
        {
            case FileOperation::OPEN:
            {
                auto sceneJsonPath = fileDialog.GetSelected().string();
                if (!neko::FileExists(sceneJsonPath))
                {
                    logDebug("[Error] Could not open " + sceneJsonPath);
                    break;
                }
                auto sceneJson = neko::LoadJson(sceneJsonPath);
                (*sceneJson)["scenePath"] = sceneJsonPath;
                sceneManager_.ParseSceneJson(*sceneJson);
                break;
            }
            case FileOperation::SAVE:
            {
                auto sceneJsonPath = fileDialog.GetSelected().string();
               sceneManager_.SaveScene(sceneJsonPath);
                logDebug("Saved scene file: " + sceneJsonPath);
                break;
            }
            default:
                break;
        }
        fileDialog.ClearSelected();
        fileDialog.Close();
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
    graphicsManager_.Render(&sceneRenderTexture_);

    sceneRenderTexture_.display();

    ImGui::SetNextWindowPos(ImVec2(0.0f, yOffset), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.2f, windowSize.y * 0.7f - yOffset), ImGuiCond_Always);
    entityViewer_.Update();

    ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.2f, yOffset), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.6f, windowSize.y * 0.7f - yOffset), ImGuiCond_Always);
    ImGui::Begin("Central Viewer", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    if (ImGui::BeginTabBar("Central Tab"))
    {
        if (ImGui::BeginTabItem("Scene Viewer"))
        {
            sceneViewer_.Update(sceneRenderTexture_);
            ImGui::EndTabItem();
        }
        if(ImGui::IsItemClicked(0))
        {
            logDebug("Clicked on Scene Viewer");
        }
        if (ImGui::BeginTabItem("Prefab Viewer"))
        {
            prefabViewer_.Update(dt);
            ImGui::EndTabItem();
        }
        if(ImGui::IsItemClicked(0))
        {
            logDebug("Clicked on Prefab Viewer");
            auto& path = sceneManager_.GetCurrentScene().scenePath;
            if(!path.empty())
            {
                sceneManager_.SaveScene(path);
            }
            else
            {
                sceneManager_.SaveScene(".tmp.scene");
            }
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

neko::SceneManager& NekoEditor::GetSceneManager()
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
          inspector_(*this),
          spriteManager_(textureManager_),
          sceneManager_(*this),
          colliderDefManager_(*this)
{

}

ColliderDefManager& NekoEditor::GetColliderDefManager()
{
    return colliderDefManager_;
}

}