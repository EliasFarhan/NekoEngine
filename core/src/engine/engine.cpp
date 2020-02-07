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

#include <engine/engine.h>
#include <engine/log.h>
#include <utilities/file_utility.h>

namespace neko
{
BasicEngine* BasicEngine::instance_ = nullptr;
BasicEngine::BasicEngine(Configuration* config)
{
    if (config != nullptr)
    {
        this->config = *config;
    }

}

BasicEngine::~BasicEngine()
{
    logDebug("Destroy Basic Engine");
}

void BasicEngine::Init()
{
    instance_ = this;
    logDebug("Current path: "+GetCurrentPath());
}

void BasicEngine::Destroy()
{
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
		Update(dt);
    }
#endif
    Destroy();
}


}