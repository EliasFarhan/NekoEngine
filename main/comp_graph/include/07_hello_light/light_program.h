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
#include "graphics/color.h"
#include "sdl_engine/sdl_camera.h"


namespace neko
{
class HelloLightProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	gl::RenderCuboid cube_{ Vec3f::zero, Vec3f::one };
	sdl::Camera3D camera_;

	
	gl::Shader lightShader_;
	gl::Shader phongShader_;

	Vec3f lightPos_;

	float time_ = 0.0f;
	const float lightDist_ = 3.5f;
	Color3 lightColor_{ 1,1,1 };
	Color3 objectColor_{ 1.0f,0.23f, 0.1f };
	float ambientStrength_ = 0.1f;
	float diffuseStrength_ = 1.0f;
	float specularStrength_ = 0.5f;
	int specularPow_ = 32;
	
};
}
