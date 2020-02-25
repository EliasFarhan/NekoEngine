#pragma once

#include "engine/window.h"
#include <SDL.h>
#include <utilities/time_utility.h>

namespace neko::sdl
{
class SdlWindow : public Window
{
public:
    virtual void Init() override;
    void Update([[maybe_unused]]seconds dt) override{};

    void Destroy() override;

    virtual void ImguiNewFrame();

protected:
    void SwapBuffer() override;

    void RenderUi() override;

    virtual void InitImGui();
    SDL_Window* window_ = nullptr;
};
}