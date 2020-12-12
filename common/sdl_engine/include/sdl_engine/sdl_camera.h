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

#include "sdl_engine.h"
#include "graphics/camera.h"

namespace neko::sdl
{

struct Camera3D : neko::Camera3D, SdlEventSystemInterface, SystemInterface
{
	void OnEvent(const SDL_Event& event) override;

	void Init() override;

	void Update(seconds dt) override;

	void Destroy() override;

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
	float cameraSpeed_ = 3.0f;
	degree_t cameraRotationSpeed_{ degree_t(10.0f) };
	float cameraFast_ = 7.0f;
	std::uint8_t cameraMovement_ = NONE;
	const Vec3f cameraOriginPos = Vec3f(0.0f, 3.0f, -3.0f);
	const EulerAngles cameraOriginAngles = EulerAngles(
		degree_t(-45.0f), degree_t(0.0f), degree_t(0.0f));
	Vec2f mouseMotion_ = Vec2f::zero;

	const float mouseMotionRatio_ = 25.0f;
};
	
}
