#pragma once

#include "sdl_engine/sdl_engine.h"
#include "triangle_command.h"

namespace neko
{
class HelloTriangleEngine : public neko::sdl::SdlGlEngine
{
public:
	explicit HelloTriangleEngine(Configuration* config = nullptr);
private:
	neko::HelloTriangleCommand helloTriangleCommand_;
};
}