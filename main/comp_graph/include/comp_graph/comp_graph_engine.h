#pragma once

#include "sdl_engine/sdl_engine.h"
#include "comp_graph/sample_browser.h"

namespace neko
{
class CompGraphEngine : public neko::sdl::SdlEngine
{
public:
	explicit CompGraphEngine(Configuration* config = nullptr);
	
    void OnEvent(const SDL_Event& event) override;

private:
	SampleBrowser sampleBrowser_;
};
}