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
#include "gl/texture.h"
#include "graphics/camera.h"

namespace neko
{
class HelloCameraProgram : public SampleProgram
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
	gl::Shader shader_;
	TextureName  textureWall_;
	Camera3D camera_;
	Mat4f projection_{ Mat4f::Identity };
	EulerAngles cameraAngles{ degree_t(0.0f), degree_t(0.0f), degree_t(0.0f) };

	Vec2f mouseMotion_ = Vec2f::zero;

	const float mouseMotionRatio_ = 50.0f;
	const float cubeDistance_ = 2.0f;
	const int cubeNumbers_ = 10;
	const Vec3f cameraOriginPos = Vec3f(0.0f, 3.0f, -3.0f);
	const EulerAngles cameraOriginAngles = EulerAngles(
		degree_t(45.0f), degree_t(0.0f), degree_t(0.0f));

	enum CameraMovement : std::uint8_t
	{
		NONE = 0u,
		UP = 1u << 0u, 
		DOWN = 1u << 1u, 
		LEFT = 1u << 2u, 
		RIGHT = 1u << 3u,
		DISABLE = 1u << 4u,
		ACCELERATE = 1u << 5u,
		MOUSE_MOVE = 1u << 6u
	};
	const float cameraSpeed_ = 2.0f;
	const float cameraFast_ = 5.0f;
	std::uint8_t cameraMovement_ = NONE;
};
}
