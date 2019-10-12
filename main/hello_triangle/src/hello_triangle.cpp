#include "sdl_engine/sdl_engine.h"

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{
    neko::Configuration config;

    neko::sdl::SdlGlEngine engine(&config);
    engine.Init();
    engine.EngineLoop();
	return EXIT_SUCCESS;
}