#include "gl/graphics.h"
#include "gl/gles3_include.h"

namespace neko::gl
{

void Gles3Renderer::ClearScreen()
{
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