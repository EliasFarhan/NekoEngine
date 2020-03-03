//
// Created by efarhan on 25.01.20.
//
#include <gl/graphics.h>
#include "gl/gles3_window.h"
#include "logger/logger_engine.h"


int main(int argc, char** argv)
{
    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
    neko::LoggerEngine engine;

    engine.SetWindowAndRenderer(&window, &renderer);

    engine.Init();
    engine.EngineLoop();
    return 0;
}