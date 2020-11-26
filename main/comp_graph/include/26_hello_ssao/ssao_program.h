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
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "gl/model.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloSsaoProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	enum SsaoFlags : std::uint8_t
	{
		NONE = 0u,
		RESIZE_SCREEN = 1u,
		ENABLE_SSAO = 1u << 1u
	};
	struct PointLight
	{
		Vec3f position = Vec3f(2.0f, 4.0f, -2.0f);
		Vec3f color = Vec3f(0.2f, 0.2f, 0.7f);

		float constant = 1.0f;
		float linear = 0.09;
		float quadratic = 0.032f;
	};
	void DestroyFramebuffer();
	void CreateFramebuffer();
	void RenderScene(const gl::Shader& shader);
	sdl::Camera3D camera_;
	gl::TextureManager textureManager_;
	TextureName container_ = INVALID_TEXTURE_NAME;
	TextureId containerId_;
	TextureName containerSpecular_ = INVALID_TEXTURE_NAME;
	TextureId containerSpecularId_;
	TextureName whiteTexture_ = 0;

	gl::Shader ssaoGeometryShader_;
	gl::Shader ssaoLightingShader_;
	gl::Shader ssaoShader_;
	gl::Shader ssaoBlurShader_;
	gl::RenderQuad screenPlane_{ Vec3f::zero, Vec2f::one * 2.0f };
	gl::RenderQuad plane_{Vec3f::zero, Vec2f::one};
	gl::ModelManager modelManager_;
	gl::ModelId modelId_ = gl::INVALID_MODEL_ID;


	unsigned int gBuffer_ = 0;
	unsigned int gPosition_ = 0;
	unsigned int gNormal_ = 0;
	unsigned int gAlbedoSpec_ = 0;
	unsigned int rbo_ = 0;

	unsigned int ssaoFbo_ = 0, ssaoBlurFbo_ = 0;
	unsigned int ssaoColorBuffer_ = 0, ssaoColorBufferBlur_ = 0;

	float ssaoRadius_ = 0.5f;
	float ssaoBias_ = 0.025f;
	TextureName noiseTexture_ = 0;
	const int maxKernelSize_ = 64;
	int kernelSize_ = maxKernelSize_;
	std::vector<Vec3f> ssaoKernel_;
	std::uint8_t flags_ = NONE;

	PointLight light_;
	
};
}