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
        //inputManager_.UpdateInputStates();
        /*if (inputManager_.IsKeyDown(SDL_SCANCODE_W))
        {
        }
        
        if (inputManager_.IsKeyHeld(SDL_SCANCODE_Q) && inputManager_.IsKeyHeld(SDL_SCANCODE_A))
        {

            std::cout << " q and a key held" << '\n';
        }*/
    	if (inputManager_.IsKeyUp(KeyCode::E))
        {
            std::cout << " e key released" << '\n';
        }
    	
    	
        if (inputManager_.IsActionButtonDown(InputAction::UP))
        {
            std::cout << " w key pressed" << '\n';
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

        ImGui::EndChild();
        ImGui::End();
    }

    void InputProgram::OnEvent(const SDL_Event& event)
    {

    }
}
