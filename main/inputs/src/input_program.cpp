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
        inputManager_.BindFromJson();
    }

    void InputProgram::Update(seconds dt)
    {
    	inputManager_.PreUserInputs();
       
        if (inputManager_.IsActionButtonDown(InputAction::UP))
        {
            std::cout << " UP action pressed\n";
        }
        if (inputManager_.IsActionButtonUp(InputAction::LEFT))
        {
            std::cout << " LEFT action released\n";
        }
        if (inputManager_.IsActionButtonHeld(InputAction::DOWN))
        {
            std::cout << " RIGHT action held\n";
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
