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

#include "gl/sdl_window.h"

#include <sstream>
#include <engine/engine.h>
#include "engine/log.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <fmt/core.h>

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#include <TracyOpenGL.hpp>
#endif
#include <engine/assert.h>

namespace neko::gl
{
void OnResizeRenderCommand::Render()
{
	logDebug(fmt::format("Resize window with new size: {}", newWindowSize_.ToString()));
	glViewport(0, 0, newWindowSize_.x, newWindowSize_.y);
}

Gles3Window::Gles3Window()
{
	flags_ = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
}

void Gles3Window::Init()
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	const auto& config = BasicEngine::GetInstance()->GetConfig();
	// Set our OpenGL version.
#ifdef WIN32
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
#else
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif


	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SdlWindow::Init();
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
#ifndef __EMSCRIPTEN__


	const std::string videoDriver = SDL_GetCurrentVideoDriver();
	logDebug(videoDriver);
#endif

	glRenderContext_ = SDL_GL_CreateContext(window_);
	MakeCurrentContext();
#ifndef __EMSCRIPTEN__
	SDL_GL_SetSwapInterval(config.flags & Configuration::VSYNC ? 1 : 0);

	if (GLEW_OK != glewInit())
	{
		neko_assert(false, "Failed to initialize OpenGL context");
	}
#else
	SDL_GL_SetSwapInterval(false);
#endif
	glCheckError();
	InitImGui();

	ImGui_ImplSDL2_InitForOpenGL(window_, &glRenderContext_);
	glCheckError();
	LeaveCurrentContext();

    const auto initRenderJob = std::make_shared<Task>([this]
    {
        MakeCurrentContext();
    });
	auto* engine = BasicEngine::GetInstance();
	engine->ScheduleTask(initRenderJob, WorkerQueue::RENDER_QUEUE_NAME);

	initRenderJob->Join();

}

void Gles3Window::InitImGui()
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	SdlWindow::InitImGui();
	ImGui_ImplSDL2_InitForOpenGL(window_, glRenderContext_);
	ImGui_ImplOpenGL3_Init("#version 300 es");

}


void Gles3Window::GenerateUiFrame()
{

#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window_);
	ImGui::NewFrame();
}


void Gles3Window::SwapBuffer()
{

#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	SDL_GL_SwapWindow(window_);
	
}

void Gles3Window::Destroy()
{

#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
    const auto leaveContext = std::make_shared<Task>([this]
		{
			LeaveCurrentContext();
		});
	BasicEngine::GetInstance()->ScheduleTask(leaveContext, WorkerQueue::RENDER_QUEUE_NAME);
	leaveContext->Join();
	MakeCurrentContext();
	ImGui_ImplOpenGL3_Shutdown();
	// Delete our OpengL context
	SDL_GL_DeleteContext(glRenderContext_);

	SdlWindow::Destroy();
}

void Gles3Window::RenderUi()
{

#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Gles3Window::OnResize(Vec2u newWindowSize)
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	onResizeCommand_.SetWindowSize(newWindowSize);
	RendererLocator::get().Render(&onResizeCommand_);

}

void Gles3Window::BeforeRenderLoop()
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	MakeCurrentContext();
	glCheckError();
}

void Gles3Window::AfterRenderLoop()
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	LeaveCurrentContext();
}

void Gles3Window::MakeCurrentContext()
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	SDL_GL_MakeCurrent(window_, glRenderContext_);
	const auto currentContext = SDL_GL_GetCurrentContext();

	logDebug(fmt::format("Current Context: {} Render Context: {} from Thread: {}",
		currentContext,
		glRenderContext_,
		std::hash < std::thread::id>{}( std::this_thread::get_id())));

	if (currentContext == nullptr)
	{
		logError(fmt::format("SDL Error: {}", SDL_GetError()));
	}
	
}

void Gles3Window::LeaveCurrentContext()
{
#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
	SDL_GL_MakeCurrent(window_, nullptr);
	const auto currentContext = SDL_GL_GetCurrentContext();

	logDebug(fmt::format(
		"Leave current context from thread: {}",
		std::hash<std::thread::id>{}(std::this_thread::get_id())));
	if (currentContext != nullptr)
	{
		logError(fmt::format("After Leave Current Context, context: {}", currentContext));
	}

}
}

