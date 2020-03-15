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
    if (inputManager_.IsKeyDown(KeyCode::W))
    {
        ImGui::BeginChild("Text");
        ImGui::Text("W KEY PRESSED");
        ImGui::EndChild();
    }
    ImGui::End();
}
	
void InputProgram::OnEvent(const SDL_Event& event)
{
	switch (event.key.type)
	{
    case SDL_KEYDOWN:
    {
        if (event.key.state == inputManager_.IsKeyDown(KeyCode::W))
        {
            std::cout << "event w key pressed" << '\n';
        }
    }
		break;
    case SDL_KEYUP:
    {
        if (event.key.type == inputManager_.IsKeyUp(KeyCode::W))
        {
            std::cout << "event w key released" << '\n';
        }
    }
		break;
	default: ;
	}
}
}