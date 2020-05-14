#include "sdl_engine/sdl_camera.h"

namespace neko::sdl
{
	void Camera3D::OnEvent(const SDL_Event& event)
	{
		switch (event.type)
		{

		case SDL_MOUSEMOTION:
		{
			mouseMotion_ = Vec2f(event.motion.xrel, event.motion.yrel) / mouseMotionRatio_;
			break;
		}
		case SDL_FINGERDOWN:
			break;
		case SDL_FINGERUP:
			break;
		case SDL_FINGERMOTION:
			break;
		default:
			break;
		}
	}

	void Camera3D::Init()
	{
		position = cameraOriginPos;
		SetDirectionFromEuler(cameraOriginAngles);
	}

	void Camera3D::Update(seconds dt)
	{
		const auto mouseState = SDL_GetMouseState(nullptr, nullptr);
		cameraMovement_ = mouseState & SDL_BUTTON(3) ?
			cameraMovement_ | MOUSE_MOVE :
			cameraMovement_ & ~MOUSE_MOVE;
		if (cameraMovement_ & MOUSE_MOVE && mouseMotion_.SquareMagnitude() > 0.001f)
		{
			Rotate(EulerAngles(
				degree_t(mouseMotion_.y),
				degree_t(mouseMotion_.x),
				degree_t(0.0f)
			));
			mouseMotion_ = Vec2f::zero;
		}
		//Checking if keys are down
		const Uint8* keys = SDL_GetKeyboardState(NULL);
		cameraMovement_ =
			keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A] ?
			cameraMovement_ | LEFT :
			cameraMovement_ & ~LEFT;
		cameraMovement_ =
			keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D] ?
			cameraMovement_ | RIGHT :
			cameraMovement_ & ~RIGHT;
		cameraMovement_ =
			keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W] ?
			cameraMovement_ | UP :
			cameraMovement_ & ~UP;
		cameraMovement_ =
			keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S] ?
			cameraMovement_ | DOWN :
			cameraMovement_ & ~DOWN;
		cameraMovement_ =
			keys[SDL_SCANCODE_LSHIFT] ?
			cameraMovement_ | ACCELERATE :
			cameraMovement_ & ~ACCELERATE;
		Vec2f cameraMove = Vec2f();
		if (cameraMovement_ & LEFT)
		{
			cameraMove.x -= 1.0f * dt.count();
		}
		if (cameraMovement_ & RIGHT)
		{
			cameraMove.x += 1.0f * dt.count();
		}
		if (cameraMovement_ & UP)
		{
			cameraMove.y += 1.0f * dt.count();
		}
		if (cameraMovement_ & DOWN)
		{
			cameraMove.y -= 1.0f * dt.count();
		}
		position +=
			(GetRight() * cameraMove.x -
				reverseDirection * cameraMove.y) *
			(cameraMovement_ & ACCELERATE ? cameraFast_ : cameraSpeed_);
	}

	void Camera3D::Destroy()
	{
	}
}
