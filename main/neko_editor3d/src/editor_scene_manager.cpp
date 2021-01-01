//
// Created by efarhan on 01/01/2021.
//

#include <editor_scene_manager.h>

namespace neko::editor
{

void EditorSceneManager::Init()
{

}

void EditorSceneManager::Update(seconds dt)
{
    hierarchy_.Update(dt);
}

void EditorSceneManager::Destroy()
{

}

void EditorSceneManager::DrawImGui()
{
    hierarchy_.DrawImGui();
}
}