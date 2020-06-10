#pragma once

#include <SDL_events.h>
#include "graphics/graphics.h"
#include "sdl_engine/sdl_engine.h"
namespace neko
{

class SampleProgram : public RenderProgram, public sdl::SdlEventSystemInterface, public DrawImGuiInterface
{
public:
    virtual ~SampleProgram() = default;
protected:
    std::mutex updateMutex_;
};
}
