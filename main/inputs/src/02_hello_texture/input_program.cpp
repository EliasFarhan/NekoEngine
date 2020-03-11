//
// Created by efarhan on 27.01.20.
//

#include <engine/engine.h>
#include "01_Input/input_program.h"

namespace neko
{

void InputProgram::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;

}

void InputProgram::Update(seconds dt)
{
    inputManager.OnPreUserInputs();
	
    if (inputManager.IsKeyDown(KeyCode::SDL_SCANCODE_W))
    {
        std::cout << "W KEY PRESSED" << '\n';
    }
	if (inputManager.IsKeyUp(KeyCode::SDL_SCANCODE_W))
    {
        std::cout << "W KEY RELEASED" << '\n';
    }
}

	

void InputProgram::Destroy()
{
}

void InputProgram::Render()
{
    
}

void InputProgram::DrawImGui()
{
    static char buf[128];

    ImGui::Begin("Input text");
    ImGui::InputText("input text", buf, IM_ARRAYSIZE(buf));
    inputManager.OnPreUserInputs();
    if (inputManager.IsKeyDown(KeyCode::SDL_SCANCODE_W))
    {
        ImGui::BeginChild("Text");
        ImGui::Text("W KEY PRESSED");
        ImGui::EndChild();
    }
    ImGui::End();
}

void InputProgram::OnEvent(const SDL_Event& event)
{

}
}