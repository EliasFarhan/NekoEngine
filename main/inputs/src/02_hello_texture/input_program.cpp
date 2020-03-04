//
// Created by efarhan on 27.01.20.
//

#include <engine/engine.h>
#include <gl/texture.h>
#include "01_Input/input_program.h"

namespace neko
{

void InputProgram::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
    SDL_StartTextInput();

}

void InputProgram::Update(seconds dt)
{
    inputManager.OnPreUserInputs();
    if (inputManager.IsKeyDown(KeyCode::W))
    {
        std::cout << "W KEY PRESSED" << '\n';
    }
}

	

void InputProgram::Destroy()
{
    SDL_StopTextInput();
}

void InputProgram::Render()
{
    
}

void InputProgram::DrawImGui()
{
	inputManager.OnPreUserInputs();
    if (inputManager.IsKeyDown(neko::KeyCode::W))
    {
        std::cout << "Touche W appuyé" << '\n';
    }
}

void InputProgram::OnEvent(const SDL_Event& event)
{

}
}