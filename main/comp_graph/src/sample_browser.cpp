//
// Created by efarhan on 26.01.20.
//

#include <imgui.h>
#include "comp_graph/sample_browser.h"


#include "01_hello_triangle/triangle_program.h"
#include "02_hello_texture/texture_program.h"
#include "03_hello_transform/transform_program.h"
#include "04_hello_coords/coords_program.h"

namespace neko
{


void SampleBrowser::Init()
{
    RegisterRenderProgram("01 Hello Triangle", std::make_unique<HelloTriangleProgram>());
    RegisterRenderProgram("02 Hello Texture", std::make_unique<TextureProgram>());
    RegisterRenderProgram("03 Hello Transform", std::make_unique<HelloTransformProgram>());
    RegisterRenderProgram("04 Hello Coords", std::make_unique<HelloCoordsProgram>());
    programs_[currentProgramIndex_]->Init();
}

void SampleBrowser::Update(seconds dt)
{
    programs_[currentProgramIndex_]->Update(dt);
    RendererLocator::get().Render(programs_[currentProgramIndex_].get());

}

void SampleBrowser::Destroy()
{
    programs_[currentProgramIndex_]->Destroy();
}

void SampleBrowser::SwitchToProgram(size_t index)
{
    const auto previousindex = currentProgramIndex_;
    currentProgramIndex_ = index;
    programs_[previousindex]->Destroy();
    programs_[currentProgramIndex_]->Init();
}


void SampleBrowser::OnEvent(const SDL_Event& event)
{
    programs_[currentProgramIndex_]->OnEvent(event);
}


void SampleBrowser::DrawImGui()
{

    ImGui::SetNextWindowPos(ImVec2(0, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("Sample Browser");
    if (ImGui::BeginCombo("Current Sample",
                          programsNames_[currentProgramIndex_].c_str())) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < programsNames_.size(); n++)
        {
	        const bool is_selected = (currentProgramIndex_ == n);
            if (ImGui::Selectable(programsNames_[n].c_str(), is_selected))
            {
                SwitchToProgram(n);
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
        }
        ImGui::EndCombo();
    }
    ImGui::End();
    ImGui::Begin("Input");

    ImGui::Text("-----------");
    ImGui::Text("KeyBoard");
    for (unsigned int keyIndex = 0;
        keyIndex != static_cast<unsigned>(sdl::KeyCode::KEYBOARD_SIZE);
        keyIndex++) {
        if (sdl::InputLocator::get().IsKeyDown(static_cast<sdl::KeyCode>(keyIndex)))
        {
            std::string text = sdl::InputLocator::get().PcInputsEnumToString(static_cast<sdl::KeyCode>(keyIndex)) + " IsKeyDown";
            ImGui::Text(text.c_str());
        }
        if (sdl::InputLocator::get().IsKeyHeld(static_cast<sdl::KeyCode>(keyIndex)))
        {
            std::string text = sdl::InputLocator::get().PcInputsEnumToString(static_cast<sdl::KeyCode>(keyIndex)) + " IsKeyHeld";
            ImGui::Text(text.c_str());
        }
        if (sdl::InputLocator::get().IsKeyUp(static_cast<sdl::KeyCode>(keyIndex)))
        {
            std::string text = sdl::InputLocator::get().PcInputsEnumToString(static_cast<sdl::KeyCode>(keyIndex)) + " IsKeyUp";
            ImGui::Text(text.c_str());
        }
    }
    ImGui::Text("-----------");
    ImGui::Text("Mouse");
    for (unsigned int mouseIndex = 0;
        mouseIndex != static_cast<unsigned>(sdl::MouseButtonCode::LENGTH);
        mouseIndex++) {
        if (sdl::InputLocator::get().IsMouseButtonDown(static_cast<sdl::MouseButtonCode>(mouseIndex)))
        {
            std::string text = sdl::InputLocator::get().PcInputsEnumToString(static_cast<sdl::MouseButtonCode>(mouseIndex)) + " IsMouseButtonDown";
            ImGui::Text(text.c_str());
        }
        if (sdl::InputLocator::get().IsMouseButtonHeld(static_cast<sdl::MouseButtonCode>(mouseIndex)))
        {
            std::string text = sdl::InputLocator::get().PcInputsEnumToString(static_cast<sdl::MouseButtonCode>(mouseIndex)) + " IsMouseButtonHeld";
            ImGui::Text(text.c_str());
        }
        if (sdl::InputLocator::get().IsMouseButtonUp(static_cast<sdl::MouseButtonCode>(mouseIndex)))
        {
            std::string text = sdl::InputLocator::get().PcInputsEnumToString(static_cast<sdl::MouseButtonCode>(mouseIndex)) + " IsMouseButtonUp";
            ImGui::Text(text.c_str());
        }
    }
    std::string mouseText = "Mouse Position : " + std::to_string((sdl::InputLocator::get().GetMousePosition().x)) + ", " + std::to_string((sdl::InputLocator::get().GetMousePosition().y)) + ")";
    ImGui::Text(mouseText.c_str());
    mouseText = "Mouse Scroll : " + std::to_string((sdl::InputLocator::get().GetMouseScroll().x)) + ", " + std::to_string((sdl::InputLocator::get().GetMouseScroll().y)) + ")";
    ImGui::Text(mouseText.c_str());
    ImGui::Text("-----------");
    ImGui::Text("Controller");
    for (unsigned int controllerIndex = 0;
        controllerIndex != static_cast<unsigned>(sdl::ControllerInputs::LENGTH);
        controllerIndex++) {
        if (sdl::InputLocator::get().IsControllerDown(static_cast<sdl::ControllerInputs>(controllerIndex)))
        {
            std::string text = sdl::InputLocator::get().ControllerInputsEnumToString(static_cast<sdl::ControllerInputs>(controllerIndex)) + " IsControllerDown";
            ImGui::Text(text.c_str());
        }
        if (sdl::InputLocator::get().IsControllerHeld(static_cast<sdl::ControllerInputs>(controllerIndex)))
        {
            std::string text = sdl::InputLocator::get().ControllerInputsEnumToString(static_cast<sdl::ControllerInputs>(controllerIndex)) + " IsControllerHeld";
            ImGui::Text(text.c_str());
        }
        if (sdl::InputLocator::get().IsControllerUp(static_cast<sdl::ControllerInputs>(controllerIndex)))
        {
            std::string text = sdl::InputLocator::get().ControllerInputsEnumToString(static_cast<sdl::ControllerInputs>(controllerIndex)) + " IsControllerUp";
            ImGui::Text(text.c_str());
        }
    }
    for (unsigned int controllerAxisIndex = 0;
        controllerAxisIndex != static_cast<unsigned>(sdl::ControllerAxis::LENGTH);
        controllerAxisIndex++) {

        std::string text = sdl::InputLocator::get().ControllerAxisEnumToString(static_cast<sdl::ControllerAxis>(controllerAxisIndex)) + " : " + std::to_string(sdl::InputLocator::get().GetJoystickAxis(static_cast<sdl::ControllerAxis>(controllerAxisIndex)));
        ImGui::Text(text.c_str());
    }
#ifdef NN_NINTENDO_SDK
    ImGui::Text("-----------");
    ImGui::Text("Switch");
    for (unsigned int switchIndex = 0;
        switchIndex != static_cast<unsigned>(sdl::SwitchInputs::LENGTH);
        switchIndex++) {
        if (sdl::InputLocator::get().IsSwitchButtonDown(static_cast<sdl::SwitchInputs>(switchIndex)))
        {
            std::string text = sdl::InputLocator::get().SwitchInputsEnumToString(static_cast<sdl::SwitchInputs>(switchIndex)) + " IsSwitchButtonDown";
            ImGui::Text(text.c_str());
        }
        if (sdl::InputLocator::get().IsSwitchButtonHeld(static_cast<sdl::SwitchInputs>(switchIndex)))
        {
            std::string text = sdl::InputLocator::get().SwitchInputsEnumToString(static_cast<sdl::SwitchInputs>(switchIndex)) + " IsSwitchButtonHeld";
            ImGui::Text(text.c_str());
        }
        if (sdl::InputLocator::get().IsSwitchButtonUp(static_cast<sdl::SwitchInputs>(switchIndex)))
        {
            std::string text = sdl::InputLocator::get().SwitchInputsEnumToString(static_cast<sdl::SwitchInputs>(switchIndex)) + " IsSwitchButtonUp";
            ImGui::Text(text.c_str());
        }
    }
#endif
    ImGui::Text("-----------");
    ImGui::Text("InputAction");
    for (unsigned int inputIndex = 0;
        inputIndex != static_cast<unsigned>(sdl::InputAction::LENGTH);
        inputIndex++) {
        if (sdl::InputLocator::get().IsActionDown(static_cast<sdl::InputAction>(inputIndex)))
        {
            std::string text = sdl::InputLocator::get().ActionEnumToString(static_cast<sdl::InputAction>(inputIndex)) + " IsActionDown";
            ImGui::Text(text.c_str());
        }
        if (sdl::InputLocator::get().IsActionHeld(static_cast<sdl::InputAction>(inputIndex)))
        {
            std::string text = sdl::InputLocator::get().ActionEnumToString(static_cast<sdl::InputAction>(inputIndex)) + " IsActionHeld";
            ImGui::Text(text.c_str());
        }
        if (sdl::InputLocator::get().IsActionUp(static_cast<sdl::InputAction>(inputIndex)))
        {
            std::string text = sdl::InputLocator::get().ActionEnumToString(static_cast<sdl::InputAction>(inputIndex)) + " IsActionUp";
            ImGui::Text(text.c_str());
        }
    }
    ImGui::End();
    programs_[currentProgramIndex_]->DrawImGui();
}

size_t SampleBrowser::RegisterRenderProgram(const std::string_view name, std::unique_ptr<SampleProgram> program)
{
    const auto index = programs_.size();
    programsNames_.emplace_back(name.data());
    programs_.push_back(std::move(program));
    return index;
}
}