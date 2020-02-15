/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include <graphics/graphics.h>
#include <algorithm>
#include <engine/globals.h>
#include <engine/window.h>
#include <engine/engine.h>
#include "engine/log.h"


namespace neko
{
Renderer::Renderer()
{
    currentCommandBuffer_.reserve(MAX_COMMAND_NMB);
    nextCommandBuffer_.reserve(MAX_COMMAND_NMB);
}


void Renderer::Render(RenderCommandInterface* command)
{
    nextCommandBuffer_.push_back(command);
}

void Renderer::RenderAll()
{
    for (auto* renderCommand : currentCommandBuffer_)
    {
        renderCommand->Render();
    }
}

void Renderer::Sync()
{
    std::unique_lock lock(renderMutex_);
    flags_ |= IS_APP_WAITING;
#ifndef EMSCRIPTEN
    cv_.wait(lock);
#endif
    std::swap(currentCommandBuffer_, nextCommandBuffer_);
    nextCommandBuffer_.clear();
    flags_ &= ~IS_APP_WAITING;
    //TODO copy all the new transform3d?
}

void Renderer::RenderLoop()
{
#ifndef EMSCRIPTEN
    flags_ |= IS_RUNNING;
    window_->LeaveCurrentContext();
    renderThread_ = std::thread([this] {
        window_->MakeCurrentContext();
        while (flags_ & IS_RUNNING)
        {
           Update();
        }
        window_->LeaveCurrentContext();
    });
#endif
}

void Renderer::Close()
{
    flags_ &= ~IS_RUNNING;
    renderThread_.join();
}

void Renderer::SetFlag(Renderer::RendererFlag flag)
{
    flags_ |= flag;
}

void Renderer::SetWindow(Window* window)
{
    window_ = window;
}

void Renderer::Update()
{
    auto* engine = BasicEngine::GetInstance();
    {
        std::lock_guard<std::mutex> lock(renderMutex_);
        window_->MakeCurrentContext();
        ClearScreen();
        engine->GenerateUiFrame();
        RenderAll();
        window_->RenderUi();
    }
    window_->SwapBuffer();

    //Sync the beginning frame with EngineLoop
    if (flags_ & IS_APP_WAITING)
    {
        cv_.notify_one();
    }
}

}

