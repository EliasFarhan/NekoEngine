#include "hello_triangle/engine.h"

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{
    neko::Configuration config;

    neko::HelloTriangleEngine engine(&config);
    engine.Init();
    engine.EngineLoop();
	return EXIT_SUCCESS;
}
