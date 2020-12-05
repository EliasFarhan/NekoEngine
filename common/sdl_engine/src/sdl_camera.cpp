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
			(rightDir * cameraMove.x -
				reverseDir * cameraMove.y) *
			(cameraMovement_ & ACCELERATE ? cameraFast_ : cameraSpeed_);
	}

	void Camera3D::Destroy()
	{
	}
}
