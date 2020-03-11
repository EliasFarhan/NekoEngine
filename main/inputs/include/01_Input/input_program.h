#pragma once

#include <input/sample_program.h>
#include <sdl_engine/sdl_input.h>
#include <imgui.h>
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"

namespace neko
{

class InputProgram : public SampleProgram
{
public:
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void Render() override;

    void DrawImGui() override;

    void OnEvent(const SDL_Event& event) override;


private:
    InputManager inputManager;
};
}