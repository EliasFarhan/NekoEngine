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

#include "gl/gles3_window.h"

#include <sstream>
#include <engine/engine.h>
#include "engine/log.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include <fmt/format.h>

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko::sdl
{
void OnResizeRenderCommand::Render()
{
	logDebug(fmt::format("Resize window with new size: {}", newWindowSize_.ToString()));
	glViewport(0, 0, newWindowSize_.x, newWindowSize_.y);
}

void Gles3Window::Init()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("GLES3WindowInit");
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

	if (!gladLoadGLES2Loader(static_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
	{
		logDebug("Failed to initialize OpenGL context\n");
		assert(false);
	}
#else
	SDL_GL_SetSwapInterval(false);
#endif
        glCheckError();
        InitImGui();
        glCheckError();
        LeaveCurrentContext();
	
	Job initRenderJob([this] { MakeCurrentContext(); });
	auto* engine = BasicEngine::GetInstance();
	engine->ScheduleJob(&initRenderJob, JobThreadType::RENDER_THREAD);

	initRenderJob.Join();
	
}

void Gles3Window::InitImGui()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("ImGuiInit");
#endif
	SdlWindow::InitImGui();
	ImGui_ImplSDL2_InitForOpenGL(window_, glRenderContext_);
	ImGui_ImplOpenGL3_Init("#version 300 es");

}


void Gles3Window::GenerateUiFrame()
{

#ifdef EASY_PROFILE_USE
	EASY_BLOCK("ImGuiGenerate");
#endif
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window_);
	ImGui::NewFrame();
}


void Gles3Window::SwapBuffer()
{

#ifdef EASY_PROFILE_USE
	EASY_BLOCK("SwapBuffer");
#endif
	SDL_GL_SwapWindow(window_);
}

void Gles3Window::Destroy()
{

#ifdef EASY_PROFILE_USE
	EASY_BLOCK("DestroyWindow");
#endif
	Job leaveContext([this]
	{
	    LeaveCurrentContext();
	});
	BasicEngine::GetInstance()->ScheduleJob(&leaveContext, JobThreadType::RENDER_THREAD);
	leaveContext.Join();
	MakeCurrentContext();
	ImGui_ImplOpenGL3_Shutdown();
	// Delete our OpengL context
	SDL_GL_DeleteContext(glRenderContext_);

	SdlWindow::Destroy();
}

void Gles3Window::RenderUi()
{

#ifdef EASY_PROFILE_USE
	EASY_BLOCK("ImGuiRender");
#endif
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void Gles3Window::OnResize(Vec2u newWindowSize)
{
	onResizeCommand_.SetWindowSize(newWindowSize);
	RendererLocator::get().Render(&onResizeCommand_);

}

void Gles3Window::BeforeRenderLoop()
{
	MakeCurrentContext();
	glCheckError();
}

void Gles3Window::AfterRenderLoop()
{
	LeaveCurrentContext();
}

void Gles3Window::MakeCurrentContext()
{
	SDL_GL_MakeCurrent(window_, glRenderContext_);
	const auto currentContext = SDL_GL_GetCurrentContext();
	std::ostringstream oss;
	oss << "Current Context: " << currentContext << " Render Context: " << glRenderContext_ << " from Thread: " << std::this_thread::get_id();
	if(currentContext == nullptr)
	{
		oss << "\nSDL Error: " << SDL_GetError();
	}
	logDebug(oss.str());

}

void Gles3Window::LeaveCurrentContext()
{
	SDL_GL_MakeCurrent(window_, nullptr);
	const auto currentContext = SDL_GL_GetCurrentContext();

	std::ostringstream oss;
	oss << "Leave current context from thread: " << std::this_thread::get_id();
	if(currentContext != nullptr)
	{
		oss << "[Error] After Leave Current Context, context: " << currentContext;
	}
	logDebug(oss.str());

}
}

#endif
