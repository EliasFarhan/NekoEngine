#pragma once

#include "sdl_engine/sdl_engine.h"
#include "input/sample_browser.h"

namespace neko
{
class InputEngine : public sdl::SdlEngine
{
public:
	explicit InputEngine(Configuration* config = nullptr);

    void Destroy() override;

private:
	SampleBrowser sampleBrowser_;
};
}