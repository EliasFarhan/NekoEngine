#pragma once

#include <SDL_events.h>
#include "graphics/graphics.h"
namespace neko
{

class SampleProgram : public RenderProgram
{
public:
    virtual void DrawUi(seconds dt) = 0;
    virtual void OnEvent(const SDL_Event& event) = 0;
protected:
    std::mutex updateMutex_;
};
}
