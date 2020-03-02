#pragma once

#include "sdl_engine/sdl_engine.h"
#include "logger/sample_browser.h"

namespace neko
{
class LoggerEngine : public sdl::SdlEngine
{
public:
	explicit LoggerEngine(Configuration* config = nullptr);
	
    void OnEvent(const SDL_Event& event) override;

private:
	SampleBrowser sampleBrowser_;
};
}