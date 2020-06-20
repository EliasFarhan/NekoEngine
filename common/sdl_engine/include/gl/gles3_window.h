#pragma once

#ifdef NEKO_GLES3

#include <mathematics/vector.h>
#include "gl/gles3_include.h"
#include "sdl_engine/sdl_window.h"
#include "graphics/graphics.h"
namespace neko::sdl
{

class OnResizeRenderCommand : public neko::RenderCommandInterface
{
public:
	void Render() override;
    void SetWindowSize(Vec2u newWindowSize) { newWindowSize_ = newWindowSize; }
protected:
    Vec2u newWindowSize_;
};
	
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
    void RenderUi() override;
    SDL_GLContext glRenderContext_{};
    OnResizeRenderCommand onResizeCommand_;
};
}
#endif