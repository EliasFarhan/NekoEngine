//
// Created by efarhan on 7/31/19.
//
#include <tools/neko_editor.h>
#include <utilities/file_utility.h>
#include <engine/log.h>

namespace editor
{
void NekoEditor::Init()
{
    BasicEngine::Init();
    sceneRenderTexture_.create(config.gameWindowSize.x, config.gameWindowSize.y);

}

void NekoEditor::Update(float dt)
{
    BasicEngine::Update(dt);
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "CTRL+N"))
            {
                for(neko::Entity entity = 0; entity < entityManager_.GetEntitiesSize();entity++)
                {
                    entityManager_.DestroyEntity(entity);
                    entityViewer_.Reset();
                }
            }
            if (ImGui::MenuItem("Open", "CTRL+O"))
            {
                fileDialog = ImGui::FileBrowser();
                fileDialog.SetPwd(config.rootPath);
                fileDialog.Open();
                fileOperationStatus_ = FileOperation::OPEN;
            }
            if (ImGui::MenuItem("Save", "CTRL+S"))
            {
                auto& path = sceneManager_.GetCurrentScene().scenePath;
                if(path.empty() || !neko::FileExists(path))
                {
                    fileDialog = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename|ImGuiFileBrowserFlags_CreateNewDir);
                    fileDialog.SetPwd(config.rootPath);
                    fileDialog.Open();
                    fileOperationStatus_ = FileOperation::SAVE;
                }
                else
                {
                    auto sceneJson = sceneManager_.SerializeScene();
                    auto sceneTxt = sceneJson.dump(4);
                    neko::WriteStringToFile(path, sceneTxt);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    fileDialog.Display();
    if(fileDialog.HasSelected())
    {
        switch (fileOperationStatus_)
        {
            case FileOperation::OPEN:
            {
                auto sceneJsonPath = fileDialog.GetSelected().string();
                if(!neko::FileExists(sceneJsonPath))
                {
                    logDebug("[Error] Could not open "+sceneJsonPath);
                    break;
                }
                auto sceneJson = neko::LoadJson(sceneJsonPath);
                sceneManager_.ParseSceneJson(*sceneJson);
                break;
            }
            case FileOperation::SAVE:
            {
                auto sceneJsonPath = fileDialog.GetSelected().string();
                auto sceneJson = sceneManager_.SerializeScene();
                auto sceneTxt = sceneJson.dump(4);
                neko::WriteStringToFile(sceneJsonPath, sceneTxt);
                sceneManager_.GetCurrentScene().scenePath = sceneJsonPath;
                sceneManager_.GetCurrentScene().sceneName = neko::GetFilename(sceneJsonPath);
                break;
            }
            default:
                break;
        }
        fileDialog.ClearSelected();
        fileDialog.Close();
    }
    sceneRenderTexture_.clear(config.bgColor);

    //Draw things into the graphics manager

    graphicsManager_.Render(&sceneRenderTexture_);
    sceneRenderTexture_.display();
    ImGui::SameLine();
    entityViewer_.Update(entityManager_, sceneManager_);
    ImGui::SameLine();
    sceneViewer_.Update(sceneRenderTexture_);

    ImGui::SameLine();
    inspector_.BeginWindow();
    inspector_.ShowEntityInfo(entityViewer_.GetSelectedEntity());
    inspector_.EndWindow();
}

void NekoEditor::Destroy()
{
    BasicEngine::Destroy();
}
}