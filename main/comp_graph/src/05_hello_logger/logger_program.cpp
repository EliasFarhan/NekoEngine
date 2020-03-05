#include <engine/engine.h>
#include "05_hello_logger/logger_program.h"
#include "imgui.h"
#include "mathematics/matrix.h"
#include "mathematics/transform.h"

namespace neko
{

void HelloLoggerProgram::Init()
{
    selectedType = 0;
    selectedLogCount = 0;
    selectedCat = 0;
    maxLogToShow = 50;
    const auto& config = BasicEngine::GetInstance()->config;
}



void HelloLoggerProgram::Update(seconds dt)
{

}

void HelloLoggerProgram::Render()
{

}

void HelloLoggerProgram::Destroy()
{

}

void HelloLoggerProgram::DrawImGui()
{
    ImGui::Begin("Logger buttons");
    if (ImGui::Button("Add Test logs")) {
        WriteTestLog();
    }
    if (ImGui::Button("Add 50 logs")) {
        Write50Logs();
    }
    if (ImGui::Button("Refresh")) {
        RefreshLogs();
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
    if (selectedCat != prevCat || selectedType != prevType || selectedLogCount != prevLogCount) {
        InsertCurrentLog();
    }

    ImGui::Separator();

    ImGui::BeginChild("logs", ImVec2(0, logScreenSize), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    logIndex = CalculateCurrentLogIndex(logsToShow.size(), ImGui::GetScrollY(), ImGui::GetScrollMaxY());
    for (int j = 0; j < logsToShow.size(); j++) {
        if (j >= logIndex - maxLogToShow && j <= logIndex + maxLogToShow) {
            ImGui::Text(logsToShow[j].log.c_str());
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

void HelloLoggerProgram::OnEvent(const SDL_Event& event)
{

}

void HelloLoggerProgram::WriteTestLog() {
    LogDebug(LogCategory::ENGINE, "Test");
    LogDebug(LogCategory::GRAPHICS, "Test");
    LogDebug(LogCategory::IO, "Test");
    LogDebug(LogCategory::MATH, "Test");
    LogDebug(LogCategory::NONE, "Test");
    LogDebug(LogCategory::SOUND, "Test");

    LogWarning(LogCategory::ENGINE, "Test");
    LogWarning(LogCategory::GRAPHICS, "Test");
    LogWarning(LogCategory::IO, "Test");
    LogWarning(LogCategory::MATH, "Test");
    LogWarning(LogCategory::NONE, "Test");
    LogWarning(LogCategory::SOUND, "Test");

    LogError(LogCategory::ENGINE, "Test");
    LogError(LogCategory::GRAPHICS, "Test");
    LogError(LogCategory::IO, "Test");
    LogError(LogCategory::MATH, "Test");
    LogError(LogCategory::NONE, "Test");
    LogError(LogCategory::SOUND, "Test");

    currentSessionLogs = Log::get().GetLogs();
    InsertCurrentLog();
}

void HelloLoggerProgram::Write50Logs() {
    for (int a = 0; a < 5000; a++) {
        LogDebug(LogCategory::NONE, "Log :" + std::to_string(a));
    }

    currentSessionLogs = Log::get().GetLogs();
    InsertCurrentLog();
}


void HelloLoggerProgram::InsertCurrentLog() {
    logsToShow.clear();
    for (int i = 0; i < currentSessionLogs.size(); i++) {

        bool correctCat = false;
        bool correctType = false;
        switch (selectedCat) {
        case 0:
            if (currentSessionLogs[i].category == LogCategory::NONE) {
                correctCat = true;
            }
            break;
        case 1:
            if (currentSessionLogs[i].category == LogCategory::ENGINE) {
                correctCat = true;
            }
            break;
        case 2:
            if (currentSessionLogs[i].category == LogCategory::MATH) {
                correctCat = true;
            }
            break;
        case 3:
            if (currentSessionLogs[i].category == LogCategory::GRAPHICS) {
                correctCat = true;
            }
            break;
        case 4:
            if (currentSessionLogs[i].category == LogCategory::IO) {
                correctCat = true;
            }
            break;
        case 5:
            if (currentSessionLogs[i].category == LogCategory::SOUND) {
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
            if (currentSessionLogs[i].type == LogType::DEBUG) {
                correctType = true;
            }
            break;
        case 1:
            if (currentSessionLogs[i].type == LogType::WARNING) {
                correctType = true;
            }
            break;
        case 2:
            if (currentSessionLogs[i].type == LogType::CRITICAL) {
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

int HelloLoggerProgram::CalculateCurrentLogIndex(int logCount, int currentY, int maxY) {
    if (maxY <= 0) { maxY = 1; }
    return (logCount * currentY) / maxY;
}

void HelloLoggerProgram::SetMaxLogsToShow() {
    logScreenSize = logScreenSizeSteps[selectedLogCount];
    maxLogToShow = maxLogCountSteps[selectedLogCount] * 0.5f;
}

void HelloLoggerProgram::RefreshLogs() {
    currentSessionLogs = Log::get().GetLogs();
    InsertCurrentLog();
}

}
