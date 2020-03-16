#pragma once

#include "sdl_engine/sdl_engine.h"
#include "comp_graph/sample_browser.h"

namespace neko
{
class CompGraphEngine : public sdl::SdlEngine
{
public:
	explicit CompGraphEngine(Configuration* config = nullptr);

    void Destroy() override;

private:
	SampleBrowser sampleBrowser_;
};
}