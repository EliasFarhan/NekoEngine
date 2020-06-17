#include "gl/graphics.h"
#include "graphics/texture.h"
#include "gl/gles3_include.h"

#include "engine/log.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif


void CheckFramebuffer()
{
    const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {

        switch (status)
        {
            case GL_FRAMEBUFFER_UNDEFINED:
                logDebug("[Error] Framebuffer is undefined!");
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                logDebug("[Error] Framebuffer is unsupported!");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                logDebug("[Error] Framebuffer has incomplete attachment!");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                logDebug("[Error] Framebuffer has incomplete missing attachment!");
                break;
            default:
                break;
        }
    }
}

namespace neko::gl
{
Gles3Renderer::Gles3Renderer() : Renderer()
{
}

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