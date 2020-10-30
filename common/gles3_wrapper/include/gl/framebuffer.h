#pragma once
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
#include "mathematics/vector.h"
#include "gl/gles3_include.h"
#include "graphics/color.h"
#include "graphics/texture.h"

namespace neko::gl
{
class Framebuffer
{
public:
    enum Type : std::uint32_t
    {
        COLOR_ATTACHMENT_0 = 1u,
        DEPTH_RBO = 1u << 1u,
        NO_DRAW = 1u << 2u,
        DEPTH_STENCIL_RBO = 1u << 3u,
        HDR = 1u << 4u,
        DEPTH_ATTACHMENT = 1u << 5u,
        DEPTH_STENCIL_ATTACHMENT = 1u << 6u,
        DEFAULT = COLOR_ATTACHMENT_0 | DEPTH_RBO
    };
	~Framebuffer();
    void Create();
    void Reload();
    void Destroy();
    void Bind() const;
    void Clear(const Color3& color);
    static void Unbind();
    /*
     * Please Use gl::Framebuffer::Type (for example Framebuffer::NO_DRAW)
     * Always Reload() to take effect
     */
    void SetType(std::uint32_t);
    /*
     * Always Reload() to take effect
     */
    void SetSize(Vec2u size);

    [[nodiscard]] TextureName GetColorTexture() const { return colorBuffer_; }
    [[nodiscard]] TextureName GetDepthTexture() const { return depthBuffer_; }
private:
    inline static unsigned int currentFramebufferBind_ = 0;
    Type frameBufferType_ = DEFAULT;
    Vec2u size_;
    unsigned int fbo_ = 0;
    unsigned int colorBuffer_ = 0;
    unsigned int depthRbo_ = 0;
    unsigned int depthBuffer_ = 0;

};
	
}
