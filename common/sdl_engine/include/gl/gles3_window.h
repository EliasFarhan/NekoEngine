#pragma once

#ifdef NEKO_GLES3

#include <atomic>
#include <mathematics/vector.h>
#include "gl/gles3_include.h"
#include "sdl_engine/sdl_window.h"
namespace neko::sdl
{
class Gles3Window : public SdlWindow
{
public:
    enum WindowFlag
    {
        ImGuiFirstFrame = 1u << 0u
    };
    void Init() override;

    void InitImGui() override;

    void Destroy() override;
    void GenerateUiFrame() override;
    void SwapBuffer() override;

    void OnResize(Vec2u newWindowSize) override;

    void MakeCurrentContext() override;
    void LeaveCurrentContext() override;
protected:
    std::atomic<std::uint8_t> flags_{0};
    void RenderUi() override;
    SDL_GLContext glContext_{};
};
}
#endif