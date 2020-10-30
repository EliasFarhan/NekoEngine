/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include "gl/graphics.h"
#include "graphics/texture.h"
#include "gl/gles3_include.h"

#include "engine/log.h"

#include <fmt/format.h>

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

void CheckGlError(const char* file, int line)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::string log;
        // Process/log the error.
        switch (err)
        {
        case GL_INVALID_ENUM:
            log += "[Error] GL Invalid Enum";
            break;
        case GL_INVALID_VALUE:
            log += "[Error] GL Invalid Value";
            break;
        case GL_INVALID_OPERATION:
            log += "[Error] GL Invalid Operation";
            break;
        case GL_OUT_OF_MEMORY:
            log += "[Error] GL Out Of Memory";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            log += "[Error] GL Invalid Framebuffer Operation";
            break;
        default:
        	continue;
        }
        logDebug(fmt::format("{} in file: {} at line: {}",log, file, line));
    }
}


void CheckFramebuffer(const char* file, int line)
{
    const auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::string log;
        switch (status)
        {
            case GL_FRAMEBUFFER_UNDEFINED:
                log+="[Error] Framebuffer is undefined!";
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                log+="[Error] Framebuffer is unsupported!";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                log+="[Error] Framebuffer has incomplete attachment!";
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                log+="[Error] Framebuffer has incomplete missing attachment!";
                break;
            default:
                return;
        }
        logDebug(fmt::format("{} in file: {} at line: {}", log, file, line));
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