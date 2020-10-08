#pragma once

/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

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
    void BeforeRenderLoop() override;
    void AfterRenderLoop() override;
protected:
    virtual void MakeCurrentContext();
    virtual void LeaveCurrentContext();
    void RenderUi() override;
    SDL_GLContext glRenderContext_{};
    OnResizeRenderCommand onResizeCommand_;
};
}
#endif