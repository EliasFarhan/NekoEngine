#pragma once

#ifdef NEKO_GLES3

#include "gl/gles3_include.h"
#include "sdl_engine/sdl_window.h"
namespace neko::sdl
{
class Gles3Window : public SdlWindow
{
public:
    void Init() override;

    void InitImGui() override;

    void OnEvent(const SDL_Event& event) override;

    void Destroy() override;
    void ClearScreen() override;
    void ImguiNewFrame() override;
    void ImguiRender() override;
    void FinishFrame() override;
protected:
    SDL_GLContext glContext_;
};
}
#endif