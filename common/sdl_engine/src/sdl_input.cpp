#include <sdl_engine/sdl_input.h>

#include <imgui.h>

#include "imgui_impl_sdl.h"

namespace neko
{

    void InputManager::Init()
    {
        memset(previousKeyStates, 0, sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));
        memcpy(currentKeyStates, SDL_GetKeyboardState(NULL), sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));
    }

    void InputManager::PreUserInputs()
    {
        memcpy(previousKeyStates, currentKeyStates, sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));
        memcpy(currentKeyStates, SDL_GetKeyboardState(NULL), sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));



        SDL_Event event;

        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
            case SDL_KEYDOWN:
                std::cout << "Bind up key : ";
                KeyBind(event);
                break;
                /*case SDL_MOUSEMOTION:
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    this->mouse_ = SDL_GetMouseState(NULL, NULL);

                    if (event.button.button == SDL_BUTTON_LEFT)
                        controllerButtonStates_[static_cast<int>(SDL_BUTTON_LEFT)] = ButtonState::DOWN;

                    else if (event.button.button == SDL_BUTTON_RIGHT)
                        controllerButtonStates_[static_cast<int>(SDL_BUTTON_RIGHT)] = ButtonState::DOWN;

                    else if (event.button.button == SDL_BUTTON_MIDDLE)
                        controllerButtonStates_[static_cast<int>(SDL_BUTTON_MIDDLE)] = ButtonState::DOWN;
                    break;

                    case SDL_CONTROLLERBUTTONDOWN:
                    this->joystick_ = SDL_JoystickBu(NULL, NULL);

                    if (event.button.button == SDL_BUTTON_LEFT)
                        controllerButtonStates_[static_cast<int>(SDL_BUTTON_LEFT)] = ButtonState::UP;

                    else if (event.button.button == SDL_BUTTON_RIGHT)
                        controllerButtonStates_[static_cast<int>(SDL_BUTTON_RIGHT)] = ButtonState::UP;

                    else if (event.button.button == SDL_BUTTON_MIDDLE)
                        controllerButtonStates_[static_cast<int>(SDL_BUTTON_MIDDLE)] = ButtonState::UP;
                    break;

                case SDL_MOUSEWHEEL:
                    break;*/

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

                // First button triggers a 0.5 second full strength rumble #2#
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

    void InputManager::BindFromJson()
    {
    }
    bool InputManager::IsKeyDown(KeyCode key) const
    {
        return currentKeyStates[static_cast<int>(key)] == 1
            && previousKeyStates[static_cast<int>(key)] == 0;
    }

    bool InputManager::IsKeyUp(KeyCode key) const
    {
        return currentKeyStates[static_cast<int>(key)] == 0
            && previousKeyStates[static_cast<int>(key)] == 1;
    }

    bool InputManager::IsKeyHeld(KeyCode key) const
    {
        return currentKeyStates[static_cast<int>(key)] == 1;
    }

    bool InputManager::IsButtonDown(ControllerButton button) const
    {
        return controllerButtonStates_[static_cast<int>(button)] == ButtonState::DOWN;
    }

    bool InputManager::IsButtonUp(ControllerButton button) const
    {
        return controllerButtonStates_[static_cast<int>(button)] == ButtonState::UP;
    }


    bool InputManager::IsActionButtonDown(InputAction button) const
    {
        const int actionIndex = static_cast<int>(button);
        const auto keyboardLength = static_cast<int>(KeyCode::LENGTH);
        if (bindPcInput[actionIndex] < keyboardLength)
        {
            return IsKeyDown(static_cast<KeyCode>(currentKeyStates[actionIndex]));
        }
        return false;
    }

    bool InputManager::IsActionButtonUp(InputAction button) const
    {
        return false;
    }

    bool InputManager::IsActionButtonHeld(InputAction button) const
    {
        return false;
    }

    Uint8 InputManager::KeyBind(SDL_Event &event)
    {    	
        return bindPcInput[event.key.keysym.sym] ;
    }

}
