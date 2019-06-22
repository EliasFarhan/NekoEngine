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
    const auto* engine = MainEngine::GetInstance();
    auto dt = engine->dt;
    const int frameIndex = (engine->frameIndex-1)%2; //Render frame is always the previous one
    ImGui::SFML::Update(*renderWindow_, dt);

    ImGui::Begin("Inspector");
    //TODO Draw inspector data
    for(auto& inspectorData : inspectorValues_[frameIndex])
    {
        ImGui::LabelText(inspectorData.first.c_str(), inspectorData.second.c_str());
    }
    ImGui::End();
    inspectorValues_[frameIndex].clear();

    ImGui::SFML::Render(*renderWindow_);
}

void Editor::Destroy()
{

}

void Editor::AddInspectorInfo(const std::string_view name, const std::string_view value)
{
    const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
    inspectorValues_[frameIndex].push_back(std::pair<std::string, std::string>(name, value));
}
}