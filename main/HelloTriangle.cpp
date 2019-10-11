#include "sdl_engine/sdl_engine.h"

int main(int argc, char** argv)
{
    neko::sdl::SdlGlEngine engine;
    engine.Init();
    engine.EngineLoop();
	return EXIT_SUCCESS;
}