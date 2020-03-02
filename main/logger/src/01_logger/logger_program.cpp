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
    ImGui::Begin("Hello Triangle Program");
    if(ImGui::Button("write")) {
        WriteLog();
    }

    ImGui::End();
}

void LoggerProgram::OnEvent(const SDL_Event& event)
{

}

void LoggerProgram::WriteLog() {
    LogManager logger;
    //LogDebug(LogCategory::ENGINE, "Engine is running");

    logger.Wait();
}


}
