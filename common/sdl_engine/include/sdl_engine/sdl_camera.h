#pragma once

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
	const float cameraSpeed_ = 3.0f;
	const float cameraFast_ = 7.0f;
	std::uint8_t cameraMovement_ = NONE;
	const Vec3f cameraOriginPos = Vec3f(0.0f, 3.0f, -3.0f);
	const EulerAngles cameraOriginAngles = EulerAngles(
		degree_t(45.0f), degree_t(0.0f), degree_t(0.0f));
	Vec2f mouseMotion_ = Vec2f::zero;

	const float mouseMotionRatio_ = 25.0f;
};
	
}
