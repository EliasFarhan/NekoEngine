//
// Created by efarhan on 13.07.19.
//

#include <MultiThreadEngine/editor.h>
#include <MultiThreadEngine/engine.h>
#include <imgui.h>
#include <imgui-SFML.h>

namespace multi
{
void MultiEditor::Init()
{

}

void MultiEditor::Update(float dt)
{
    const auto* engine = neko::BasicEngine::GetInstance<MainEngine>();
    const neko::Index frameIndex = (engine->frameIndex - 1) % 2; //Render frame is always the previous one
    auto* renderWindow = engine->renderWindow.get();
    ImGui::SFML::Update(*renderWindow, engine->clockDeltatime);

    ImGui::Begin("Inspector");
    //Draw inspector data
    for (auto& inspectorData : inspectorValues_[frameIndex])
    {
        ImGui::LabelText(inspectorData.first.c_str(), "%s", inspectorData.second.c_str());
    }
    ImGui::End();
    inspectorValues_[frameIndex].clear();

    ImGui::SFML::Render(*renderWindow);
}

void MultiEditor::Destroy()
{

}

void MultiEditor::AddInspectorInfo(const std::string_view name, const std::string_view value)
{
    const neko::Index frameIndex = neko::BasicEngine::GetInstance<MainEngine>()->frameIndex % 2;
    inspectorValues_[frameIndex].push_back(std::pair<std::string, std::string>(name, value));
}
}