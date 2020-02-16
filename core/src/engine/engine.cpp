/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <chrono>
#include <sstream>

#include <engine/engine.h>
#include <engine/log.h>
#include <utilities/file_utility.h>
#include "graphics/graphics.h"
#include "imgui.h"
#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko
{
BasicEngine* BasicEngine::instance_ = nullptr;

BasicEngine::BasicEngine(Configuration* config)
{
	if (config != nullptr)
	{
		this->config = *config;
	}

#ifdef EASY_PROFILE_USE
	EASY_PROFILER_ENABLE;
#endif
}

BasicEngine::~BasicEngine()
{
	logDebug("Destroy Basic Engine");

#ifdef EASY_PROFILE_USE
	profiler::dumpBlocksToFile("Neko_Profile.prof");
#endif
}

void BasicEngine::Init()
{

#ifdef EASY_PROFILE_USE
	EASY_FUNCTION(profiler::colors::Magenta);
#endif
	instance_ = this;
	logDebug("Current path: " + GetCurrentPath());
}

void BasicEngine::Update(seconds dt)
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Basic Engine Update");
#endif
	{
#ifdef EASY_PROFILE_USE
		EASY_BLOCK("Application Update");
#endif
		ManageEvent();
		updateAction_.Execute(dt);
	}
#ifdef EMSCRIPTEN
	renderer_->Update();
#endif

	renderer_->Sync();
}

void BasicEngine::Destroy()
{
	renderer_->Close();
	instance_ = nullptr;
}

static std::chrono::time_point<std::chrono::system_clock> clock;
#ifdef EMSCRIPTEN
void EmLoop(void* arg)
{
	BasicEngine* engine = static_cast<BasicEngine*>(arg);
	const auto start = std::chrono::system_clock::now();
	const auto dt = std::chrono::duration_cast<seconds>(start - clock);
	clock = start;
	(engine)->Update(dt);
}
#endif

void BasicEngine::EngineLoop()
{
	isRunning_ = true;

	renderer_->RenderLoop();
	clock = std::chrono::system_clock::now();
#ifdef EMSCRIPTEN
	// void emscripten_set_main_loop(em_callback_func func, int fps, int simulate_infinite_loop);

	emscripten_set_main_loop_arg(&EmLoop, this, 0, 1);
#else
	while (isRunning_)
	{
		const auto start = std::chrono::system_clock::now();
		const auto dt = std::chrono::duration_cast<seconds>(start - clock);
		clock = start;
		dt_ = dt.count();
		Update(dt);
	}
#endif
	Destroy();
}

void BasicEngine::SetWindowAndRenderer(Window* window, Renderer* renderer)
{
	window_ = window;
	renderer_ = renderer;
	renderer_->SetWindow(window);
	RendererLocator::provide(renderer);
}

void BasicEngine::GenerateUiFrame()
{
	ImGui::Begin("Neko Window");

	std::ostringstream oss;
	oss << "App FPS: " << 1.0f / GetDeltaTime() << '\n'
		<< "Render FPS: " << 1.0f / renderer_->GetDeltaTime() << '\n';
	ImGui::Text(oss.str().c_str());
	ImGui::End();
	drawUiAction_.Execute();
}


}
