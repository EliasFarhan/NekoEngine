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
        const float LTaxis = inputManager_.GetAxisValue(ControllerAxis::LEFT_TRIGGER);
        const float RTaxis = inputManager_.GetAxisValue(ControllerAxis::RIGHT_TRIGGER);
        const float LSVaxis = inputManager_.GetAxisValue(ControllerAxis::VERTICAL_LEFT_AXIS);
    	const float RSVaxis = inputManager_.GetAxisValue(ControllerAxis::VERTICAL_RIGHT_AXIS);
        const float LSHaxis = inputManager_.GetAxisValue(ControllerAxis::HORIZONTAL_LEFT_AXIS);
        const float RSHaxis = inputManager_.GetAxisValue(ControllerAxis::HORIZONTAL_RIGHT_AXIS);
    	
        ImGui::Begin("Input text");
        ImGui::InputText("input text", buf, IM_ARRAYSIZE(buf));
        ImGui::BeginChild("Axis value");
        ImGui::Text("Left trigger axis = %f", LTaxis);
        /*ImGui::Text("Right trigger axis = %f", RTaxis);
        ImGui::Text("Left stick vertical axis = %f", LSVaxis);
        ImGui::Text("Right stick vertical axis = %f", RSVaxis);
        ImGui::Text("Left stick horizontal axis = %f", LSHaxis);*/
        ImGui::Text("Right stick horizontal axis = %f", RSHaxis);
        ImGui::EndChild();
        ImGui::End();
    }

    void InputProgram::OnEvent(const SDL_Event& event)
    {

    }
}
