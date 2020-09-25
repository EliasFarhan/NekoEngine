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
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloIblProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	enum IblFlags : std::uint8_t
	{
		NONE = 0u,
		FIRST_FRAME = 1u,
		SHOW_IRRADIANCE = 1u << 1u,
		ENABLE_IRRADIANCE = 1u << 2u,
		SHOW_PREFILTER = 1u << 3u,
		ENABLE_IBL_SPECULAR = 1u << 4u,
		ENABLE_SCHLICK_ROUGHNESS = 1u << 5u,
		
	};
	struct Light
	{
		Vec3f position;
		Vec3f color;
	};
	void GenerateCubemap();
	void GenerateDiffuseIrradiance();
	void GeneratePrefilter();
	void GenerateLUT();
	
	std::array<Light, 4> lights_;
	gl::TextureManager textureManager_;
	TextureName hdrTexture_ = INVALID_TEXTURE_NAME;
	TextureId hdrTextureId_;
	gl::RenderSphere sphere_{Vec3f::zero, 1.0f};
	gl::RenderCuboid skybox_{ Vec3f::zero, Vec3f::one*2.0f };
	gl::RenderQuad quad_{ Vec3f::zero, Vec2f::one*2.0f };

	gl::Shader equiToCubemap_;
	gl::Shader skyboxShader_;
	gl::Shader pbrShader_;
	gl::Shader irradianceShader_;
	gl::Shader prefilterShader_;
	gl::Shader brdfShader_;

	unsigned int captureFbo_ = 0, captureRbo_ = 0;
	TextureName envCubemap_ = 0;
	TextureName irradianceMap_ = 0;
	TextureName brdfLUTTexture_ = 0;

	sdl::Camera3D camera_;
	Vec3f baseColor_ = { 1.0f,0.5f,0.5f };
	float spacing_ = 2.5f;
	std::uint8_t flags_ = NONE;

	const std::array<Vec3f, 6> viewDirs =
	{
		{

		Vec3f(1.0f,  0.0f,  0.0f),
		Vec3f(-1.0f,  0.0f,  0.0f),
		Vec3f(0.0f,  1.0f,  0.0f),
		Vec3f(0.0f, -1.0f,  0.0f),
		Vec3f(0.0f,  0.0f,  1.0f),
		Vec3f(0.0f,  0.0f, -1.0f),
		}
	};
	const std::array<Vec3f, 6> upDirs =
	{
		{
		Vec3f(0.0f, 1.0f,  0.0f),
		Vec3f(0.0f, 1.0f,  0.0f),
		Vec3f(0.0f,  0.0f,  -1.0f),
		Vec3f(0.0f,  0.0f, 1.0f),
		Vec3f(0.0f, 1.0f,  0.0f),
		Vec3f(0.0f, 1.0f,  0.0f),
		}
	};
	TextureName prefilterMap_ = 0;
};
}
