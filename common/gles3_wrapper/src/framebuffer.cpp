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

#include "gl/framebuffer.h"

namespace neko::gl
{
Framebuffer::~Framebuffer()
{
    if (fbo_ != 0)
    {
        Destroy();
    }
}

void Framebuffer::Create()
{
    glCheckError();
    glGenFramebuffers(1, &fbo_);
    Bind();
    glCheckError();

    if(frameBufferType_ & DEPTH_ATTACHMENT)
    {
        glGenTextures(1, &depthBuffer_);
        glBindTexture(GL_TEXTURE_2D, depthBuffer_);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16,
            size_.x, size_.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer_, 0);
        GLenum drawBuffers = GL_NONE;
        glDrawBuffers(1, &drawBuffers);
        glReadBuffer(GL_NONE);
        glBindTexture(GL_TEXTURE_2D, 0);
        glCheckError();
    }
    else if(frameBufferType_ & DEPTH_RBO)
    {
        glGenRenderbuffers(1, &depthRbo_);
        glBindRenderbuffer(GL_RENDERBUFFER, depthRbo_);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size_.x, size_.y);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRbo_);
        glCheckError();
    }
    if(frameBufferType_ & NO_DRAW)
    {
        GLenum drawBuffers = GL_NONE;
        glDrawBuffers(1, &drawBuffers);
        glReadBuffer(GL_NONE);
        glCheckError();
    }
    else if(frameBufferType_ & COLOR_ATTACHMENT_0)
    {
        glGenTextures(1, &colorBuffer_);
        glBindTexture(GL_TEXTURE_2D, colorBuffer_);
        glTexImage2D(GL_TEXTURE_2D, 0, frameBufferType_ & HDR ? GL_RGB16F : GL_RGB8, size_.x, size_.y, 0, GL_RGB,
            frameBufferType_ & HDR ? GL_FLOAT : GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer_, 0);
        glCheckError();
    }

    glCheckFramebuffer();
    Unbind();
}

void Framebuffer::Reload()
{
    //Just unbind before Destroy...
    if (fbo_ != 0 && currentFramebufferBind_ == fbo_)
        Unbind();
    Destroy();
    Create();
}

void Framebuffer::Destroy()
{
    if (fbo_)
        glDeleteFramebuffers(1, &fbo_);
    if (colorBuffer_)
        glDeleteTextures(1, &colorBuffer_);
    if (depthBuffer_)
        glDeleteTextures(1, &depthBuffer_);
    if (depthRbo_)
        glDeleteRenderbuffers(1, &depthRbo_);
    fbo_ = 0;
    colorBuffer_ = 0;
    depthBuffer_ = 0;
    depthRbo_ = 0;
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_);
    currentFramebufferBind_ = fbo_;
}

void Framebuffer::Clear(const Color3& color)
{
    glClearColor(color.x, color.y, color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    currentFramebufferBind_ = 0;
}

void Framebuffer::SetType(std::uint32_t type)
{
    frameBufferType_ = static_cast<Type>(type);
}

void Framebuffer::SetSize(Vec2u size)
{
    size_ = size;
}
}
