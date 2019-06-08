#include <engine/editor.h>

#include <imgui.h>
#include <imgui-SFML.h>
#include <engine/engine.h>
#include <graphics/graphics.h>

#include <utility>

namespace neko
{
void Editor::Init()
{

}

void Editor::Update()
{
    auto dt = MainEngine::GetInstance()->dt;
    const int frameIndex = graphicsManager->frameIndex%2;
    ImGui::SFML::Update(*renderWindow, dt);

    ImGui::Begin("Inspector");
    //TODO Draw inspector data
    for(auto& inspectorData : inspectorValues[frameIndex])
    {
        ImGui::LabelText(inspectorData.first.c_str(), inspectorData.second.c_str());
    }
    ImGui::End();
    inspectorValues[frameIndex].clear();

    ImGui::SFML::Render(*renderWindow);
}

void Editor::Destroy()
{

}

void Editor::AddInspectorInfo(const std::string_view name, const std::string_view value)
{
    const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
    inspectorValues[frameIndex].push_back(std::pair<std::string, std::string>(name, value));
}
}