#include "gl/graphics.h"

#include "gl/gles3_include.h"
#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko::gl
{

void Gles3Renderer::ClearScreen()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Clear Screen");
#endif
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Gles3Renderer::BeforeRenderLoop()
{
    Renderer::BeforeRenderLoop();
	
    glEnable(GL_DEPTH_TEST);
}

void Gles3Renderer::AfterRenderLoop()
{
    Renderer::AfterRenderLoop();
}
}