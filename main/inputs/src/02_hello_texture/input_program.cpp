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
    if (inputManager.IsKeyDown(SDLK_w))
    {
        ImGui::BeginChild("Text");
        ImGui::Text("W KEY PRESSED");
        ImGui::EndChild();
    }
    ImGui::End();
}

void InputProgram::OnEvent(const SDL_Event& event)
{
	switch (event.type)
	{
    case SDL_KEYDOWN:
    {
        if (event.key.state == inputManager.IsKeyDown(SDLK_w))
        {
            std::cout << "event w key pressed" << '\n';
        }
    }
		break;
    case SDL_KEYUP:
    {
        if (event.key.state == inputManager.IsKeyUp(SDLK_w))
        {
            std::cout << "event w key released" << '\n';
        }
    }
		break;
	}
}
}