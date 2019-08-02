#include <tools/log_viewer.h>
#include <engine/log.h>
#include <imgui.h>

namespace editor
{

void LogViewer::Update()
{
    auto& logHistory = getLog();
    for(auto& log : logHistory)
    {
        if(log.find("[Error]") != std::string_view::npos)
        {
            ImGui::TextColored(ImColor(255,0,0), "%s", log.data());
        }
        else if(log.find("[Warning]") != std::string_view::npos)
        {
            ImGui::TextColored(ImColor(255,255,0), "%s", log.data());
        }
        else
        {
            ImGui::Text("%s", log.data());
        }
    }
}
}