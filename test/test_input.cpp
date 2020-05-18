
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <random>
#include <gtest/gtest.h>
#include <sdl_engine/sdl_engine.h>

#include "gl/gles3_window.h"
#include "gl/graphics.h"

TEST(Engine, TestInput)
{
    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
    neko::sdl::SdlEngine engine(nullptr);

    engine.SetWindowAndRenderer(&window, &renderer);

    engine.Init();
    engine.EngineLoop();
}