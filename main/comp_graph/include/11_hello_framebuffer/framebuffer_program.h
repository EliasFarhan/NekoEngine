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
#include "comp_graph/sample_program.h"
#include "sdl_engine/sdl_camera.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "gl/shape.h"

namespace neko
{

class HelloFramebufferProgram : public SampleProgram
{
public:
    enum class PostProcessingType
    {
        NO_POSTPROCESS,
        INVERSE,
        GRAYSCALE,
        BLUR,
        EDGE_DETECTION,
        LENGTH
    };

    HelloFramebufferProgram();

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void OnEvent(const SDL_Event& event) override;
private:
    sdl::Camera3D camera_;

    gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};
    gl::Shader modelShader_;
    gl::TextureManager textureManager_;
    TextureName containerTexture_ = INVALID_TEXTURE_NAME;
    TextureId containerTextureId_;

    gl::RenderQuad screenFrame_;
    gl::Shader screenShader_;
    gl::Shader screenInverseShader_;
    gl::Shader screenGrayscaleShader_;
    gl::Shader screenBlurShader_;
    gl::Shader screenEdgeDetectionShader_;

    PostProcessingType postProcessingType_ = PostProcessingType::NO_POSTPROCESS;

    unsigned int fbo_;
    unsigned int rbo_;
    unsigned int fboColorBufferTexture_;
    bool hasScreenResize_ = false;
};


}