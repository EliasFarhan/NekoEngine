#pragma once

#include "sdl_engine/sdl_engine.h"
#include "triangle_program.h"

namespace neko
{
class HelloTriangleEngine : public neko::sdl::SdlEngine
{
public:
	explicit HelloTriangleEngine(Configuration* config = nullptr);
private:
	neko::HelloTriangleCommand helloTriangleCommand_;
};
}