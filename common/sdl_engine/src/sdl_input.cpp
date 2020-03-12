#include <sdl_engine/sdl_input.h>
namespace neko
{
    InputManager::InputManager() = default;

    InputManager::~InputManager() = default;

    void InputManager::OnPreUserInputs()
	{
    	//for (auto& keyStates : keyPressedStates)
        for (size_t i = 0; i < SDL_NUM_SCANCODES ; i++)
        {
            
            if (keyPressedStates[i] == SDL_KEYDOWN)
            {
                keyPressedStates[i] == SDL_PRESSED;
            }
        }
        for (size_t i = 0; i < SDL_LASTEVENT; i++) 
        {
	        if (buttonPressedStates[i] == SDL_MOUSEBUTTONDOWN)
	        {
                buttonPressedStates[i] == SDL_RELEASED;
	        }
	        if (buttonPressedStates[i] == SDL_MOUSEBUTTONDOWN)
	        {
                buttonPressedStates[i] == SDL_PRESSED;
	        }
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
            // SDL2's new way of handling input
            case SDL_KEYDOWN:
            {
                this->keyboard_ = SDL_GetKeyboardState(nullptr);
                const auto index = event.key.keysym.scancode;
            	keyPressedStates[index] = SDL_KEYDOWN;
            }
            	break;
            case SDL_KEYUP:
            {
                this->keyboard_ = SDL_GetKeyboardState(nullptr);
                const auto index = event.key.keysym.scancode;
                keyPressedStates[index] = SDL_KEYUP;
			}
                break;
            case SDL_MOUSEMOTION:
                mousePos_.x = -event.motion.x;
                mousePos_.y = -event.motion.y;
                break;

            case SDL_MOUSEBUTTONDOWN:
                this->mouse_ = SDL_GetMouseState(NULL, NULL);

                if (event.button.button == SDL_BUTTON_LEFT)
                    buttonPressedStates[static_cast<int>(SDL_BUTTON_LEFT)] = SDL_MOUSEBUTTONDOWN;

                else if (event.button.button == SDL_BUTTON_RIGHT)
                    buttonPressedStates[static_cast<int>(SDL_BUTTON_RIGHT)] = SDL_MOUSEBUTTONDOWN;

                else if (event.button.button == SDL_BUTTON_MIDDLE)
                    buttonPressedStates[static_cast<int>(SDL_BUTTON_MIDDLE)] = SDL_MOUSEBUTTONDOWN;
                break;

            case SDL_MOUSEBUTTONUP:
                this->mouse_ = SDL_GetMouseState(NULL, NULL);

                if (event.button.button == SDL_BUTTON_LEFT)
                    buttonPressedStates[static_cast<int>(SDL_BUTTON_LEFT)] = SDL_MOUSEBUTTONUP;

                else if (event.button.button == SDL_BUTTON_RIGHT)
                    buttonPressedStates[static_cast<int>(SDL_BUTTON_RIGHT)] = SDL_MOUSEBUTTONUP;

                else if (event.button.button == SDL_BUTTON_MIDDLE)
                    buttonPressedStates[static_cast<int>(SDL_BUTTON_MIDDLE)] = SDL_MOUSEBUTTONUP;
                break;

            case SDL_MOUSEWHEEL:
                scrollX = event.wheel.y;
                scrollY = event.wheel.y;

                scrollUpdate = true;
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
                switch (event.jaxis.axis)
            	{
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

        if (!scrollUpdate) {
            scrollX = 0;
            scrollY = 0;
        }
        else
        {
	        scrollUpdate = false;
        }


    }

    bool InputManager::IsKeyDown(KeyCode key) const
    {
	    return keyPressedStates[static_cast<int>(key)] == SDL_KEYDOWN;
    }

    bool InputManager::IsKeyUp(KeyCode key) const
    {
	    return keyPressedStates[static_cast<int>(key)] == SDL_KEYUP;
    }

    bool InputManager::IsKeyHeld(KeyCode key) const
    {
        if (!keyboard_ || keyboard_ == nullptr)
        {
            return false;
        }
        const auto keyHeld = static_cast<int>(key);
	    return keyboard_[keyHeld] != 0;
    }

    Vec2f InputManager::GetMousePosition() const
    {
	    return mousePos_;
    }

    bool InputManager::IsButtonDown(ButtonCode button) const
    {
	    return buttonPressedStates[static_cast<int>(button)] == SDL_MOUSEBUTTONDOWN;
    }

    bool InputManager::IsButtonUp(ButtonCode button) const
    {
	    return buttonPressedStates[static_cast<int>(button)] == SDL_MOUSEBUTTONUP;
    }
	
}

