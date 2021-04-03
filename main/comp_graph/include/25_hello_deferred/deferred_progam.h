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
#include <comp_graph/sample_program.h>
#include "sdl_engine/sdl_camera.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "gl/model.h"

namespace neko
{
class HelloDeferredProgram : public SampleProgram
{
public:

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void OnEvent(const SDL_Event& event) override;

private:
    enum DeferredFlags : std::uint8_t
    {
        NONE = 0u,
        RESIZE_SCREEN = 1u,
        FORWARD_RENDERING = 1u << 1u
    };

    struct PointLight
    {
        Vec3f position;
        Vec3f color;
    };

    PointLight lights_[32];


    void CreateFramebuffer();
    void RenderScene(const gl::Shader& shader);

    sdl::Camera3D camera_;
    gl::Shader deferredShader_;
    gl::Shader lightingShader_;
    gl::Shader forwardShader_;

    gl::RenderQuad floor_{Vec3f::zero, Vec2f::one*10.0f};
    gl::RenderQuad screenQuad_{Vec3f::zero, Vec2f::one*2.0f};
    gl::RenderCuboid cube_{Vec3f(0.0f,0.5f,0.0f), Vec3f::one};
    gl::ModelManager modelManager_;
    gl::ModelId modelId_ = gl::INVALID_MODEL_ID;

    gl::TextureManager textureManager_;
    gl::TextureName container_ = gl::INVALID_TEXTURE_NAME;
    gl::TextureId containerId_;
    gl::TextureName containerSpecular_ = gl::INVALID_TEXTURE_NAME;
    gl::TextureId containerSpecularId_;
    gl::TextureName whiteTexture_ = 0;

    unsigned int gBuffer_ = 0;
    unsigned int gPosition_ = 0;
    unsigned int gNormal_ = 0;
    unsigned int gAlbedoSpec_ = 0;
    unsigned int rbo_ = 0;
    std::uint8_t flags_ = NONE;
};

}