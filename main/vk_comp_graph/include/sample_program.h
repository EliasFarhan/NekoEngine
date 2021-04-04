#pragma once

#include <graphics/graphics.h>
#include <vk/vk_window.h>
#include <sdl_engine/sdl_engine.h>

namespace neko::vk
{
class SampleProgram : public RenderProgram, public sdl::SdlEventSystemInterface, public DrawImGuiInterface
{
public:
    SampleProgram(VkWindow& window);
    virtual ~SampleProgram() = default;

protected:
    VkWindow& window_;
};
}