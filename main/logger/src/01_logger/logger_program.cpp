#include <cmath>
#include <imgui.h>
#include <engine/engine.h>
#include "01_logger/logger_program.h"
#include <engine/log.h>
namespace neko
{
void LoggerProgram::Init()
{
    selectedType = 0;
    selectedLogCount = 0;
    selectedCat = 0;
    maxLogToShow = 50;
    const auto& config = BasicEngine::GetInstance()->config; 
}

void LoggerProgram::Update(seconds dt)
{
}

void LoggerProgram::Render()
{

}

void LoggerProgram::Destroy()
{

}

void LoggerProgram::DrawUi()
{

    ImGui::Begin("Logger buttons");
    if(ImGui::Button("Add Test logs")) {
        WriteTestLog();
    }
    if (ImGui::Button("Add 50 logs")) {
        Write50Logs();
    }
    ImGui::End();

    ImGui::Begin("Logs");
    int prevCat = selectedCat;
    int prevType = selectedType;
    int prevLogCount = selectedLogCount;

    ImGui::Combo("Category", &selectedCat, caterogies, IM_ARRAYSIZE(caterogies));
    ImGui::Combo("Type", &selectedType, types, IM_ARRAYSIZE(types));
    ImGui::Combo("Log Count", &selectedLogCount, logCount, IM_ARRAYSIZE(logCount));
    SetMaxLogsToShow();
    if(selectedCat != prevCat || selectedType != prevType || selectedLogCount != prevLogCount) {
        InsertCurrentLog();
    }

    ImGui::Separator();

    ImGui::BeginChild("logs", ImVec2(0,logScreenSize), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    logIndex = CalculateCurrentLogIndex(logsToShow.size(), ImGui::GetScrollY(), ImGui::GetScrollMaxY());
        for (int j = 0; j < logsToShow.size(); j++) {
            if (j >= logIndex - maxLogToShow && j <= logIndex + maxLogToShow) {
                ImGui::Text(logsToShow[j].log.c_str());
            }
        }
    ImGui::EndChild();

    ImGui::End();
}

void LoggerProgram::OnEvent(const SDL_Event& event)
{

}

void LoggerProgram::WriteTestLog() {
    LogDebug(LogCategories::ENGINE, "Test");
    LogDebug(LogCategories::GRAPHICS, "Test");
    LogDebug(LogCategories::IO, "Test");
    LogDebug(LogCategories::MATH, "Test");
    LogDebug(LogCategories::NONE, "Test");
    LogDebug(LogCategories::SOUND, "Test");

    LogWarning(LogCategories::ENGINE, "Test");
    LogWarning(LogCategories::GRAPHICS, "Test");
    LogWarning(LogCategories::IO, "Test");
    LogWarning(LogCategories::MATH, "Test");
    LogWarning(LogCategories::NONE, "Test");
    LogWarning(LogCategories::SOUND, "Test");

    LogError(LogCategories::ENGINE, "Test");
    LogError(LogCategories::GRAPHICS, "Test");
    LogError(LogCategories::IO, "Test");
    LogError(LogCategories::MATH, "Test");
    LogError(LogCategories::NONE, "Test");
    LogError(LogCategories::SOUND, "Test");

    logger.Wait();
    currentSessionLogs = logger.GetLogs();
    InsertCurrentLog();

}

void LoggerProgram::Write50Logs() {
    for (int a = 0; a < 50; a++) {
        LogDebug(LogCategories::NONE, "Log :" + std::to_string(a));
    }

    logger.Wait();
    currentSessionLogs = logger.GetLogs();
    InsertCurrentLog();
}


void LoggerProgram::InsertCurrentLog() {
    logsToShow.clear();
    for (int i = 0; i < currentSessionLogs.size(); i++) {

        bool correctCat = false;
        bool correctType = false;
        switch (selectedCat) {
        case 0:
            if (currentSessionLogs[i].category == LogCategories::NONE) {
                correctCat = true;
            }
            break;
        case 1:
            if (currentSessionLogs[i].category == LogCategories::ENGINE) {
                correctCat = true;
            }
            break;
        case 2:
            if (currentSessionLogs[i].category == LogCategories::MATH) {
                correctCat = true;
            }
            break;
        case 3:
            if (currentSessionLogs[i].category == LogCategories::GRAPHICS) {
                correctCat = true;
            }
            break;
        case 4:
            if (currentSessionLogs[i].category == LogCategories::IO) {
                correctCat = true;
            }
            break;
        case 5:
            if (currentSessionLogs[i].category == LogCategories::SOUND) {
                correctCat = true;
            }
            break;
        case 6:
            correctCat = true;
            break;
        default:
            correctCat = false;
            break;
        }
        switch (selectedType) {
        case 0:
            if (currentSessionLogs[i].type == LogTypes::DEBUG) {
                correctType = true;
            }
            break;
        case 1:
            if (currentSessionLogs[i].type == LogTypes::WARNING) {
                correctType = true;
            }
            break;
        case 2:
            if (currentSessionLogs[i].type == LogTypes::CRITICAL) {
                correctType = true;
            }
            break;
        case 3:
            correctType = true;
            break;
        default:
            correctType = false;
            break;
        }

        if (correctCat && correctType) {
            logsToShow.push_back(currentSessionLogs[i]);
        }
    }
}

int LoggerProgram::CalculateCurrentLogIndex(int logCount, int currentY, int maxY) {
    if (maxY <= 0) { maxY = 1; }
    return (logCount * currentY) / maxY;
}

void LoggerProgram::SetMaxLogsToShow() {
    logScreenSize = logScreenSizeSteps[selectedLogCount];
    maxLogToShow = maxLogCountSteps[selectedLogCount] * 0.5f;
}




}
