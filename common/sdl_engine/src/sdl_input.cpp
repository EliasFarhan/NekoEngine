#include <sdl_engine/sdl_input.h>
namespace neko
{

    void InputManager::PreUserInputs()
	{
        const size_t keyboardLength = static_cast<size_t>(KeyCode::LENGTH);

        for (size_t i = 0; i < keyboardLength; i++)
        {
            
            if (keyPressedStates_[i] == ButtonState::DOWN)
            {
                keyPressedStates_[i] = ButtonState::HELD;
            }
            if (keyPressedStates_[i] == ButtonState::UP)
            {
                keyPressedStates_[i] = ButtonState::NONE;
            }
        }

        const size_t controllerLength = static_cast<size_t>(ControllerCode::LENGTH);

        for (size_t i = 0; i < controllerLength; i++) 
        {
	        if (buttonPressedStates_[i] == ButtonState::DOWN)
	        {
                buttonPressedStates_[i] = ButtonState::HELD;
	        }
	        if (buttonPressedStates_[i] == ButtonState::UP)
	        {
                buttonPressedStates_[i] = ButtonState::NONE;
	        }
        }

        const size_t switchLength = static_cast<size_t>(SwitchButton::LENGTH);
        for (size_t i = 0; i < switchLength; i++)
        {
            if (switchButtonState_[i] == ButtonState::DOWN)
            {
                switchButtonState_[i] = ButtonState::HELD;
            }
            if (switchButtonState_[i] == ButtonState::UP)
            {
                switchButtonState_[i] = ButtonState::NONE;
            }
        }

        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            switch (event.type)
            {
            // SDL2's new way of handling input
            case SDL_KEYDOWN:
            {
                this->states_ = SDL_GetKeyboardState(nullptr);
                const auto index = event.key.keysym.scancode;
            	keyPressedStates_[index] = ButtonState::DOWN;
            }
            	break;
            case SDL_KEYUP:
            {
                this->states_ = SDL_GetKeyboardState(nullptr);
                const auto index = event.key.keysym.scancode;
                keyPressedStates_[index] = ButtonState::UP;
			}
                break;
            case SDL_MOUSEMOTION:
                mousePos_.x = -event.motion.x;
                mousePos_.y = -event.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                this->mouse_ = SDL_GetMouseState(NULL, NULL);

                if (event.button.button == SDL_BUTTON_LEFT)
                    buttonPressedStates_[static_cast<int>(SDL_BUTTON_LEFT)] = ButtonState::DOWN;

                else if (event.button.button == SDL_BUTTON_RIGHT)
                    buttonPressedStates_[static_cast<int>(SDL_BUTTON_RIGHT)] = ButtonState::DOWN;

                else if (event.button.button == SDL_BUTTON_MIDDLE)
                    buttonPressedStates_[static_cast<int>(SDL_BUTTON_MIDDLE)] = ButtonState::DOWN;
                break;

            case SDL_MOUSEBUTTONUP:
                this->mouse_ = SDL_GetMouseState(NULL, NULL);

                if (event.button.button == SDL_BUTTON_LEFT)
                    buttonPressedStates_[static_cast<int>(SDL_BUTTON_LEFT)] = ButtonState::UP;

                else if (event.button.button == SDL_BUTTON_RIGHT)
                    buttonPressedStates_[static_cast<int>(SDL_BUTTON_RIGHT)] = ButtonState::UP;

                else if (event.button.button == SDL_BUTTON_MIDDLE)
                    buttonPressedStates_[static_cast<int>(SDL_BUTTON_MIDDLE)] = ButtonState::UP;
                break;

            case SDL_MOUSEWHEEL:
                break;

            case SDL_JOYDEVICEREMOVED:

                SDL_JoystickClose(joystick_);

                std::cout << "No more joystick\n";
                break;

            case SDL_JOYDEVICEADDED:
            {
                const int device = event.jdevice.which;
                joystick_ = SDL_JoystickOpen(device);
                if (joystick_ != nullptr) {
                    SDL_assert(SDL_JoystickFromInstanceID(SDL_JoystickInstanceID(joystick_)) == joystick_);
                }
            }
            break;

            case SDL_JOYAXISMOTION:
                break;
            case SDL_JOYHATMOTION:
            {
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


    }

    void InputManager::BindfromJson()
    {
    }

    bool InputManager::IsKeyDown(KeyCode key) const
    {
	    return keyPressedStates_[static_cast<int>(key)] == ButtonState::DOWN;
    }

    bool InputManager::IsKeyUp(KeyCode key) const
    {
	    return keyPressedStates_[static_cast<int>(key)] == ButtonState::UP;
    }

    bool InputManager::IsKeyHeld(KeyCode key) const
    {
        if (!states_ || states_ == nullptr)
        {
            return false;
        }
        const auto keyHeld = static_cast<int>(key);
	    return states_[keyHeld] != 0;
    }

    bool InputManager::IsButtonDown(ControllerCode button) const
    {
	    return buttonPressedStates_[static_cast<int>(button)] == ButtonState::DOWN;
    }

    bool InputManager::IsButtonUp(ControllerCode button) const
    {
	    return buttonPressedStates_[static_cast<int>(button)] == ButtonState::DOWN;
    }

	bool InputManager::IsActionButtonDown(ActionInput button) const
	{
		return false;
	}

	bool InputManager::IsActionButtonUp(ActionInput button) const
	{
		return false;
	}

	bool InputManager::IsActionButtonHeld(ActionInput button) const
	{
		return false;
	}
	
}

