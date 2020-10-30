#pragma once

#include "sdl_engine/sdl_engine.h"
#include "comp_net/sample_browser.h"

namespace neko::net
{
class RdtEngine : public sdl::SdlEngine
{
public:
    explicit RdtEngine(Configuration* config = nullptr);

    void Destroy() override;

private:
    SampleBrowser sampleBrowser_;
};
}