#include <cmath>
#include <imgui.h>
#include <engine/engine.h>
#include "01_logger/logger_program.h"
#include <engine/log.h>
namespace neko
{
void LoggerProgram::Init()
{
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
    const char* caterogies[] = { "NONE","ENGINE","MATH","GRAPHICS","IO","SOUND", "ALL" };
    const char* types[] = { "DEBUG","WARNING","CRITICAL", "ALL"};
    const char* logCount[] = { "10","20","50", "100", "ALL" };


    ImGui::Begin("Logger buttons");
    if(ImGui::Button("Write logs")) {
        WriteLog();
    }
    ImGui::End();

    ImGui::Begin("Logs");
    static int selectedCat = 0;
    static int selectedType = 0;
    static int selectedLogCount = 0;

    ImGui::Combo("Category", &selectedCat, caterogies, IM_ARRAYSIZE(caterogies));
    ImGui::Combo("Type", &selectedType, types, IM_ARRAYSIZE(types));
    ImGui::Combo("LogCount (NOT IMPLEMENTED)", &selectedLogCount, logCount, IM_ARRAYSIZE(logCount));

    ImGui::Separator();
    ImGui::BeginChild("logs");
    for (int i = 0; i < currentSessionLogs.size(); i++) {
        bool correctCat = false;
        bool correctType = false;

        switch(selectedCat) {
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
            ImGui::Text(currentSessionLogs[i].log.c_str());
        }
    }
    ImGui::EndChild();
    ImGui::End();
}

void LoggerProgram::OnEvent(const SDL_Event& event)
{

}

void LoggerProgram::WriteLog() {
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


    logger.Wait();
    currentSessionLogs = logger.GetLogs();

}



}
