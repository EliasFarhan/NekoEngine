#pragma once

#include "engine/window.h"
#include <SDL.h>
#include <utilities/time_utility.h>

namespace neko::sdl
{
class SdlWindow : neko::Window
{
public:
    virtual void Init() override;
    virtual void InitImGui();
    void Update([[maybe_unused]]seconds dt) override{};

    virtual void OnEvent(const SDL_Event& event);
    void Destroy() override;

    //Functions used by the SDL Engine update
    virtual void ClearScreen() = 0;
    virtual void ImguiNewFrame();
    virtual void ImguiRender();
    virtual void FinishFrame(){};

protected:
    SDL_Window* window_ = nullptr;
};
}