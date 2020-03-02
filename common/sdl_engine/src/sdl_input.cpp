#include <sdl_engine/sdl_input.h>

namespace neko
{
    InputManager::InputManager()
    {
    }

    void InputManager::OnPreUserInputs() {

        keyPressedUp.clear();
        keyPressedDown.clear();
    	
        for (size_t i = 0; i < static_cast<int>(ButtonCode::MOUSE_MAX); i++) {
            buttonUp_[i] = false;
            buttonDown_[i] = false;
        }

        if (!joyAxisUpdate) {
            joyAxisLX = 0;
            joyAxisLY = 0;
            joyAxisRX = 0;
            joyAxisRY = 0;
        }
        else { joyAxisUpdate = false; }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            switch (event.type)
            {
            case SDL_QUIT:
                break;

            case  SDL_WINDOWEVENT:
            {
                if (event.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                }
                break;
            }

#pragma region keyboard
            // SDL2's new way of handling input
            case SDL_TEXTINPUT:
                break;

            case SDL_KEYDOWN: {
                this->keyboard_ = SDL_GetKeyboardState(nullptr);

                const int index = event.key.keysym.scancode;
                keyPressedDown.push_back(static_cast<KeyCode>(index));
            }
                            break;

            case SDL_KEYUP: {
                this->keyboard_ = SDL_GetKeyboardState(nullptr);

                const int index = event.key.keysym.scancode;
                keyPressedUp.push_back(static_cast<KeyCode>(index));
                /*if (IsKeyUp(neko::KeyCode::W))
                {
                    std::cout << "Touche W appuyé" << '\n';
                }*/
            }
                          break;
#pragma endregion

#pragma region Mouse
            case SDL_MOUSEMOTION:
                mousePos_.x = -event.motion.x;
                mousePos_.y = -event.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                this->mouse_ = SDL_GetMouseState(NULL, NULL);

                if (event.button.button == SDL_BUTTON_LEFT)
                    buttonDown_[static_cast<int>(ButtonCode::LEFT)] = true;

                else if (event.button.button == SDL_BUTTON_RIGHT)
                    buttonDown_[static_cast<int>(ButtonCode::RIGHT)] = true;

                else if (event.button.button == SDL_BUTTON_MIDDLE)
                    buttonDown_[static_cast<int>(ButtonCode::MIDDLE)] = true;
                break;

            case SDL_MOUSEBUTTONUP:
                this->mouse_ = SDL_GetMouseState(NULL, NULL);

                if (event.button.button == SDL_BUTTON_LEFT)
                    buttonUp_[static_cast<int>(ButtonCode::LEFT)] = true;

                else if (event.button.button == SDL_BUTTON_RIGHT)
                    buttonUp_[static_cast<int>(ButtonCode::RIGHT)] = true;

                else if (event.button.button == SDL_BUTTON_MIDDLE)
                    buttonUp_[static_cast<int>(ButtonCode::MIDDLE)] = true;
                break;

            case SDL_MOUSEWHEEL:
                scrollX = event.wheel.y;
                scrollY = event.wheel.y;

                scrollUpdate = true;
                break;
#pragma endregion

            case SDL_JOYDEVICEREMOVED:

                SDL_JoystickClose(joystick_);

                std::cout << "No more joystick\n";
                break;

            case SDL_JOYDEVICEADDED: {
                const int device = event.jdevice.which;
                joystick_ = SDL_JoystickOpen(device);
                if (joystick_ != nullptr) {
                    SDL_assert(SDL_JoystickFromInstanceID(SDL_JoystickInstanceID
                    (joystick_)) == joystick_);
                }

            }
                                   break;

            case SDL_JOYAXISMOTION:
                switch (event.jaxis.axis) {
                case 0:
                    joyAxisLX = event.jaxis.value;
                    break;
                case 1:
                    joyAxisLY = event.jaxis.value;
                    break;
                case 2:
                    joyAxisRX = event.jaxis.value;
                    break;
                case 3:
                    joyAxisRY = event.jaxis.value;
                    break;
                }
                joyAxisUpdate = true;
                break;
            case SDL_JOYHATMOTION: {
                std::string s = "Joystick " + std::to_string(event.jhat.which) +
                    " hat " + std::to_string(event.jhat.hat) +
                    " value:";
                if (event.jhat.value == SDL_HAT_CENTERED)
                    s += " centered";
                if (event.jhat.value & SDL_HAT_UP)
                    s += " up";
                if (event.jhat.value & SDL_HAT_RIGHT)
                    s += " right";
                if (event.jhat.value & SDL_HAT_DOWN)
                    s += " down";
                if (event.jhat.value & SDL_HAT_LEFT)
                    s += " left";
            }
                                 break;
            case SDL_JOYBALLMOTION:

                break;
            case SDL_JOYBUTTONDOWN:

                /* First button triggers a 0.5 second full strength rumble */
                if (event.jbutton.button == 0) {
                    SDL_JoystickRumble(joystick_, 0xFFFF, 0xFFFF, 500);
                }
                break;
            case SDL_JOYBUTTONUP:
                break;
                /* Fall through to signal quit */
            case SDL_FINGERDOWN:
                break;

            case SDL_FINGERMOTION:
                break;

            case SDL_FINGERUP:
                break;
            default:
                break;
            }
         }

        if (!scrollUpdate) {
            scrollX = 0;
            scrollY = 0;
        }
        else { scrollUpdate = false; }


    }
	
    bool InputManager::IsKeyDown(KeyCode key)
    {
	    for (KeyCode keyPressedDown_ : keyPressedDown)
	    {
		    if (keyPressedDown_ == key)
		    {
                return true;
		    }
	    }
    	return false;
        

    }

	bool InputManager::IsKeyUp(KeyCode key)
	{
        for (KeyCode keyPressedUp_ : keyPressedUp)
        {
            if (keyPressedUp_ == key)
            {
                return true;
            }
        }
    	return false;
	}

}

