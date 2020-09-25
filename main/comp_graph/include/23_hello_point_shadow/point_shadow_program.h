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
#include "gl/shape.h"
#include "gl/texture.h"

namespace neko
{
class HelloPointShadowProgram : public SampleProgram
{
public:
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void OnEvent(const SDL_Event& event) override;

private:
    struct Transform
    {
        Vec3f position = Vec3f::zero;
        Vec3f scale = Vec3f::one;
        Vec3f axis = Vec3f::up;
        radian_t angle = radian_t(0.0f);
    };



    void RenderScene(const gl::Shader& shader);

    sdl::Camera3D camera3D_;
    gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};

    gl::Shader lightCubeShader_;
    Camera3D lightCamera_;
    gl::Shader simpleDepthShader_;

    gl::Shader cubeShader_;
    gl::TextureManager textureManager_;
    TextureName cubeTexture_ = INVALID_TEXTURE_NAME;
    TextureId cubeTextureId_;

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFbo_;
    unsigned int depthCubemap_;
    float dt_ = 0.0f;
    float bias_ = 0.005f;

    std::array<Transform, 5> transforms_ =
            {
                    {
                            {Vec3f(4.0f, -3.5f, 0.0f), Vec3f(0.5f)},
                            {Vec3f(2.0f, 3.0f, 1.0f), Vec3f(0.75f)},
                            {Vec3f(-3.0f, -1.0f, 0.0f), Vec3f(0.5f)},
                            {Vec3f(-1.5f, 1.0f, 1.5f), Vec3f(0.5f)},
                            {Vec3f(-1.5f, 2.0f, -3.0f), Vec3f(0.75f), Vec3f(1, 0, 1).Normalized(), degree_t(60.0f)}
                    }
            };
};
}