//
// Created by efarhan on 27.01.20.
//

#include <engine/engine.h>
#include <input/input_program.h>
#include <imgui.h>

namespace neko
{

    void InputProgram::Init()
    {
        const auto& config = BasicEngine::GetInstance()->config;
        
    }

    void InputProgram::Update(seconds dt)
    {
    	inputManager_.PreUserInputs();
       
        if (inputManager_.IsKeyHeld(KeyCode::Q) && inputManager_.IsKeyHeld(KeyCode::A))
        {
            std::cout << " q and a key held" << '\n';
        }
    	if (inputManager_.IsKeyUp(KeyCode::E))
        {
            std::cout << " e key released" << '\n';
        }
    	
        if (inputManager_.IsActionButtonDown(InputAction::UP))
        {
            std::cout << " UP action pressed" << '\n';
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
        ImGui::End();
    }

    void InputProgram::OnEvent(const SDL_Event& event)
    {

    }
}
