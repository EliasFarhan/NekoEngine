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
#include "gl/shader.h"
#include "gl/shape.h"
#include "gl/model.h"
#include "sdl_engine/sdl_camera.h"


namespace neko
{
class HelloShadowProgram : public SampleProgram
{
public:
    void OnEvent(const SDL_Event& event) override;

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;
private:
	enum ShadowFlags : std::uint8_t
	{
		NONE = 0u,
		ENABLE_SHADOW = 1u,
		ENABLE_BIAS = 1u << 1u,
		ENABLE_PETER_PANNING = 1u << 2u,
		ENABLE_OVER_SAMPLING = 1u << 3u,
		ENABLE_PCF = 1u << 4u
	};
    struct DirectionalLight
    {
        Vec3f lightPos = Vec3f::one*10.0f;
        Vec3f lightDir = (-Vec3f::one).Normalized();
    };

    struct Transform
    {
        Vec3f position = Vec3f::zero;
        Vec3f scale = Vec3f::one;
        radian_t angle = radian_t(0.0f);
        Vec3f axis = Vec3f::up;
    };
    void RenderScene(const gl::Shader& shader);
	
    gl::RenderQuad floor_{Vec3f::zero, Vec2f::one*5.0f};
    gl::TextureManager textureManager_;
    TextureName floorTexture_ = INVALID_TEXTURE_NAME;
    TextureId floorTextureId_;

    gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};
    std::array<Transform, 4> cubeTransforms_ = {
        {
	        {Vec3f(0.0f, 3.0f, 0.0f), Vec3f(0.5f)},
        	{Vec3f(2.0f, 1.5f, 1.0f), Vec3f(0.5f)},
        	{Vec3f(4.0f, 0.25f, 4.0f), Vec3f(0.5f)},
        	{Vec3f(-1.0f, 1.5f, 2.0f), Vec3f(0.25f), degree_t(60.0f), Vec3f(1,0,1).Normalized()}
        }
    };
    gl::ModelManager modelManager_;
    gl::ModelId modelId_ = gl::INVALID_MODEL_ID;


    gl::Shader simpleDepthShader_;
    gl::Shader modelShader_;

    sdl::Camera3D camera_;
    Camera2D depthCamera_;

    unsigned int depthMapFbo_ = 0;
    unsigned int depthMap_ = 0;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    DirectionalLight light_;

    float shadowBias_ = 0.0005f;
    std::uint8_t flags_ = NONE;
};
}