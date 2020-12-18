#include "app.h"
#include "gl/gles3_window.h"
#include "sdl_engine/sdl_engine.h"
#include "gl/graphics.h"

int main([[maybe_unused]]int argc, [[maybe_unused]] char** argv)
{
    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
    neko::Filesystem filesystem;
    neko::Configuration config;
    config.flags = neko::Configuration::NONE;

    neko::sdl::SdlEngine engine(filesystem, config);
    engine.SetWindowAndRenderer(&window, &renderer);

    neko::PerfApp app;
    engine.RegisterSystem(app);

    engine.Init();
    engine.EngineLoop();

    return EXIT_SUCCESS;
}