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

#include "sdl_engine/sdl_camera.h"

namespace neko::sdl
{
	void Camera3D::OnEvent(const SDL_Event& event)
	{
		switch (event.type)
		{
		    case SDL_WINDOWEVENT:
            {
                if(event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    SetAspect(event.window.data1, event.window.data2);
                }
                break;
            }
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
		WorldLookAt(Vec3f::zero);
	}

	void Camera3D::Update(seconds dt)
	{
		const auto mouseState = SDL_GetMouseState(nullptr, nullptr);
		
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
		
		if (SDL_IsGameController(0))
		{
			auto* controller = SDL_GameControllerOpen(0);
			const auto move = Vec2f(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX),
				-SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY)) / std::numeric_limits<short>::max();
			if (move.SquareMagnitude() > 0.1f)
			{
				cameraMove += move * dt.count() * cameraSpeed_;
			}
            const short rightX = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX);
            const short rightY = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY);
			
			auto rotate = Vec2f(rightX, rightY) / std::numeric_limits<short>::max();
			if (rotate.SquareMagnitude() > 0.1f)
			{ 
				rotate = rotate * cameraRotationSpeed_.value() * dt.count();
				Rotate(EulerAngles(
					degree_t(rotate.y),
					-degree_t(rotate.x),
					degree_t(0.0f)
				)) ;
			}
			SDL_GameControllerClose(controller);
		}
		
		position +=
			(rightDir * cameraMove.x -
				reverseDir * cameraMove.y) *
			(cameraMovement_ & ACCELERATE ? cameraFast_ : cameraSpeed_);

		cameraMovement_ = mouseState & SDL_BUTTON(3) ?
			cameraMovement_ | MOUSE_MOVE :
			cameraMovement_ & ~MOUSE_MOVE;
		if (cameraMovement_ & MOUSE_MOVE && mouseMotion_.SquareMagnitude() > 0.001f)
		{
			const auto rotate = Vec2f(mouseMotion_.x, mouseMotion_.y)  * cameraRotationSpeed_.value() * dt.count();
			Rotate(EulerAngles(
				degree_t(rotate.y),
				degree_t(rotate.x),
				degree_t(0.0f)
			));
			mouseMotion_ = Vec2f::zero;
		}
	}

	void Camera3D::Destroy()
	{
	}
}
