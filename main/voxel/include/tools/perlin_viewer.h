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

#include <gl/shape.h>
#include <gl/font.h>
#include <gl/shader.h>
#include "engine/system.h"
#include "graphics/graphics.h"
#include "gl/gles3_include.h"
#include "graphics/texture.h"

namespace neko::voxel
{

class PerlinViewer : public SystemInterface, public DrawImGuiInterface, public RenderCommandInterface
{
public:
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

private:
    void ReloadTexture();

    std::int32_t currentOctave_ = 1;
    float currentScale_ = 1.0f;
    inline static const size_t textureSize_ = 1024;
    std::vector<std::array<float, textureSize_>> perlinValues_;
    neko::TextureName perlinTexture_ = neko::INVALID_TEXTURE_NAME;

    neko::gl::RenderQuad renderQuad_{Vec3f::zero, Vec2f::one};
    neko::gl::Shader viewerShader_;

};
}