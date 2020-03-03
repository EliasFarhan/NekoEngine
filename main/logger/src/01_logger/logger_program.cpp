#include <imgui.h>
#include <engine/engine.h>
#include "01_logger/logger_program.h"
#include <engine/log.h>
namespace neko
{
void LoggerProgram::DrawUi()
{
    if (ImGui::Begin("Logs", &isActive_, ImGuiWindowFlags_MenuBar))
    {
	    if (ImGui::BeginMenuBar())
	    {
		    if (ImGui::BeginMenu("Severity"))
		    {
			    for (uint8_t i = 1; i < static_cast<uint8_t>(LogTypes::LENGTH); ++i)
			    {
				    const bool isSelected = currentTypes_ & 1 << i - 1;
			    	if (ImGui::MenuItem(LogTypeToString(static_cast<LogTypes>(i)).c_str(),
						"", isSelected))
			    	{
						if (isSelected) {
							currentTypes_ &= ~(1 << i - 1);
						}
						else {
							currentTypes_ |= 1 << i - 1;
						}
			    	}
			    }
				ImGui::EndMenu();
		    }

		    if (ImGui::BeginMenu("Type"))
		    {
				for (uint8_t i = 1; i < static_cast<uint8_t>(LogCategories::LENGTH); ++i)
				{
					const bool isSelected = currentCategories_ & 1 << i - 1;
					if (ImGui::MenuItem(LogCategoryToString(static_cast<LogCategories>(i)).c_str(),
						"", isSelected))
					{
						if (isSelected) {
							currentCategories_ &= ~(1 << i - 1);
						}
						else {
							currentCategories_ |= 1 << i - 1;
						}
					}
				}
				ImGui::EndMenu();
		    }
	    	
			if (ImGui::Button("Generate Test Logs")) {
				GenerateTestLogs();
			}
	    	
			ImGui::EndMenuBar();
	    }

		ImGui::Text("Number of logs to display (0 for all)");
	    ImGui::InputScalar("##LogCount", ImGuiDataType_U32, &logCount_);

	    ImGui::Separator();
    	
	    ImGui::BeginChild("logs");
		auto& logs = Log::get().GetLogs();
		const uint32_t logsSize = logs.size();
	    const uint32_t logsOffset = logCount_ > 0 ?
			std::clamp(logCount_, 0u, logsSize) : logsSize;
	    for (uint32_t i = logsSize - logsOffset; i < logsSize; ++i)
	    {
		    const uint8_t category = static_cast<uint8_t>(logs[i].category);
		    const uint8_t type = static_cast<uint8_t>(logs[i].type);
		    if (currentCategories_ & 1 << category - 1 &&
				currentTypes_ & 1 << type - 1)
		    {
				ImGui::Text(logs[i].log.c_str());
		    }
	    }
	    ImGui::EndChild();
    	
		ImGui::End();
    }
}

void LoggerProgram::GenerateTestLogs() const
{
	LogDebug(LogCategories::NONE, "Debug");
    LogDebug(LogCategories::ENGINE, "Engine Debug");
    LogDebug(LogCategories::GRAPHICS, "Graphics Debug");
    LogDebug(LogCategories::IO, "I/O Debug");
    LogDebug(LogCategories::MATH, "Math Debug");
    LogDebug(LogCategories::SOUND, "Sound Debug");

	LogWarning(LogCategories::NONE, "Warning");
	LogWarning(LogCategories::ENGINE, "Engine Warning");
	LogWarning(LogCategories::GRAPHICS, "Graphics Warning");
	LogWarning(LogCategories::IO, "I/O Warning");
	LogWarning(LogCategories::MATH, "Math Warning");
	LogWarning(LogCategories::SOUND, "Sound Warning");

	LogError(LogCategories::NONE, "Error");
	LogError(LogCategories::ENGINE, "Engine Error");
	LogError(LogCategories::GRAPHICS, "Graphics Error");
	LogError(LogCategories::IO, "I/O Error");
	LogError(LogCategories::MATH, "Math Error");
	LogError(LogCategories::SOUND, "Sound Error");
}



}
