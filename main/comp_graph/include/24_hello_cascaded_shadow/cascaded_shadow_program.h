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
#include "gl/texture.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/model.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloCascadedShadowProgram : public SampleProgram
{
public:
    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;
    void DrawImGui() override;
    void Render() override;
    void OnEvent(const SDL_Event& event) override;
private:

    enum CascadedShadowFlags : std::uint8_t
    {
        NONE = 0u,
        ENABLE_CASCADE_COLOR = 1u,
        ENABLE_AABB_CASCADE = 1u << 1u
    };

    struct DirectionalLight
    {
        Vec3f position = Vec3f::zero;
        Vec3f direction = -Vec3f::one;
        Mat4f lightSpaceMatrix;
    };

    [[nodiscard]] Camera2D CalculateOrthoLight(float cascadeNear, float cascadeFar, Vec3f lightDir) const;
    void ShadowPass(int cascadeIndex);
    void RenderScene(const gl::Shader& shader);

    sdl::Camera3D camera_;

    gl::Shader simpleDepthShader_;
    gl::Shader shadowShader_;

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned fbo_ = 0;
    std::array<unsigned, 3> shadowMaps_{};
    float cascadedNearRatio_ = 0.1f;
    float cascadedMiddleRatio_ = 0.6f;
    float shadowBias_ = 0.005f;
    std::uint8_t flags_ = NONE;
    std::array<DirectionalLight, 3> lights_;

    gl::RenderQuad plane_{ Vec3f::zero, Vec2f::one };
    gl::ModelManager modelManager_;
    gl::ModelId dragonModelId_ = gl::INVALID_MODEL_ID;

    gl::TextureManager textureManager_;
    TextureName brickWall_ = INVALID_TEXTURE_NAME;
    TextureId brickWallId_;
    TextureName whiteTexture_ = 0;
};
}
