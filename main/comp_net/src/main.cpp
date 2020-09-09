#include <gl/gles3_window.h>
#include <gl/graphics.h>
#include "comp_net/engine.h"

int main(int argc, char** argv)
{
    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
    neko::net::CompNetEngine engine;

    engine.SetWindowAndRenderer(&window, &renderer);

    engine.Init();
    engine.EngineLoop();
    return EXIT_SUCCESS;
}