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
#include "gl/model.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloWaterProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	void CreateFramebuffer();
	void CreateDepthbuffer();
	
	gl::ModelManager modelManager_;
	gl::ModelId modelId_ = gl::INVALID_MODEL_ID;

	gl::RenderQuad quad_{Vec3f::zero, Vec2f::one};
	gl::RenderCuboid skyboxCube_{Vec3f::zero, Vec3f::one*2.0f};
	gl::TextureName skyboxTexture_ = 0;
	sdl::Camera3D camera_;
	gl::Shader modelShader_;
	gl::Shader waterShader_;
	gl::Shader skyboxShader_;

	gl::TextureManager textureManager_;
	gl::TextureName dudvTexturer_;
	gl::TextureId dudvTexturerId_;
	gl::TextureName normalMap_;
	gl::TextureId normalMapId_;

	const Vec2u reflectionFrameSize = Vec2u(480, 240);
	unsigned int reflectionFramebuffer_;
	unsigned int reflectionColorBuffer_;
	unsigned int reflectionDepthBuffer_;
	const Vec2u refractionFrameSize = Vec2u(480, 240);
	unsigned int refractionFramebuffer_;
	unsigned int refractionColorBuffer_;
	unsigned int refractionDepthBuffer_;

	unsigned int depthFramebuffer_;
	unsigned int depthBuffer_;
	float dt_ = 0.0f;
	float waterHeight_ = 1.0f;
	bool resizeScreen_ = false;
};
}
