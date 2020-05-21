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
#include <chrono>
#include <sstream>
#include <engine/globals.h>
#include <engine/window.h>
#include <engine/engine.h>
#include "engine/log.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko
{
Renderer::Renderer() :
	renderAllJob_( [this]
	{
#if defined(__ANDROID__)
		window_->MakeCurrentContext();
#endif
		auto* engine = BasicEngine::GetInstance();
		PreRender();
		ClearScreen();
		engine->GenerateUiFrame();
		RenderAll();
		window_->RenderUi();
	} ),
	syncJob_([this] {Sync(); })
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
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("RenderAllCPU");
#endif
	for (auto* renderCommand : currentCommandBuffer_)
	{
		renderCommand->Render();
	}
}

void Renderer::ScheduleJobs()
{
	auto* engine = BasicEngine::GetInstance();
	engine->ScheduleJob(&syncJob_, JobThreadType::RENDER_THREAD);
	engine->ScheduleJob(&renderAllJob_, JobThreadType::RENDER_THREAD);
}

void Renderer::Sync()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Swapping Render Command");
#endif
	std::swap(currentCommandBuffer_, nextCommandBuffer_);
	nextCommandBuffer_.clear();
	//TODO copy all the new transform3d?

}

void Renderer::PreRender()
{
	using namespace std::chrono_literals;
	std::chrono::microseconds availableLoadingTime(8000);
	while (availableLoadingTime > 0us)
	{
		std::chrono::time_point<std::chrono::system_clock> start = 
			std::chrono::system_clock::now();


		Job* job = nullptr;
		{
			std::lock_guard<std::mutex> lock(preRenderJobsMutex_);
			if (!preRenderJobs_.empty())
			{
				job = preRenderJobs_.front();
				preRenderJobs_.erase(preRenderJobs_.begin());
			}
		}

		if (job != nullptr && job->CheckDependenciesStarted())
		{
			job->Execute();
		}
		else
		{
			break;
		}

		std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		availableLoadingTime -= duration;
	}
}

void Renderer::Destroy()
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("ClosingFromEngine");
#endif
	flags_ &= ~IS_RUNNING;
}

void Renderer::SetFlag(Renderer::RendererFlag flag)
{
	flags_ |= flag;
}

void Renderer::SetWindow(Window* window)
{
	window_ = window;
}

void Renderer::AddPreRenderJob(Job* job)
{
	std::lock_guard<std::mutex> lock(preRenderJobsMutex_);
	preRenderJobs_.push_back(job);
}

void Renderer::ResetJobs()
{
	syncJob_.Reset();
	renderAllJob_.Reset();
}

void Renderer::BeforeRenderLoop()
{
#if !defined(NEKO_SAMETHREAD)
	window_->MakeCurrentContext();
#endif
}
void Renderer::AfterRenderLoop()
{
#if !defined(NEKO_SAMETHREAD)
	window_->LeaveCurrentContext();
#endif
}



}

