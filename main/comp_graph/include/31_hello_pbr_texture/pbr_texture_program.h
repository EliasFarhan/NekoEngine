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
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloPbrTextureProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	struct Light
	{
		Vec3f position;
		Vec3f color;
	};
	std::array<Light, 4> lights_;
	gl::RenderSphere sphere_{ Vec3f::zero, 1.0f };
	gl::Shader pbrShader_;
	sdl::Camera3D camera_;
	gl::TextureManager textureManager_;
	TextureName albedo_;
	TextureId albedoId_;
	TextureName normal_;
	TextureId normalId_;
	TextureName metallic_;
	TextureId metallicId_;
	TextureName roughness_;
	TextureId roughnessId_;
	unsigned int ao_ = 0;


	bool gammaCorrect_ = true;
};
}
