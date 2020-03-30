#include <sdl_engine/sdl_input.h>

#include <imgui.h>

#include "utilities/json_utility.h"


namespace neko
{

    void InputManager::Init()
    {
        BindFromJson();
        memset(previousKeyStates, 0, sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));
        memcpy(currentKeyStates, SDL_GetKeyboardState(NULL), sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));

        memset(currentControllerBtnStates, 0, sizeof(Uint8) * static_cast<size_t>(ControllerButton::LENGTH));
        memset(previousControllerBtnStates, 0, sizeof(Uint8) * static_cast<size_t>(ControllerButton::LENGTH));
    	
        SDL_JoystickEventState(SDL_ENABLE);
    }

    void InputManager::PreUserInputs()
    {
        memcpy(previousKeyStates, currentKeyStates, sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));
        memcpy(currentKeyStates, SDL_GetKeyboardState(NULL), sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));
    	
        memcpy(&previousControllerBtnStates, &currentControllerBtnStates, sizeof(Uint8) * static_cast<size_t>(ControllerButton::LENGTH));

        SDL_GameControllerUpdate();

        const auto controllerLength = static_cast<int>(ControllerButton::LENGTH);
        for (int b = 0; b < controllerLength; ++b) {
            currentControllerBtnStates[b] = SDL_JoystickGetButton(joystick_, b);
        }
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {

            switch (event.type)
            {
            case SDL_KEYDOWN:
            {
            }
            break;
            case SDL_JOYDEVICEREMOVED:

                SDL_JoystickClose(joystick_);

                std::cout << "No more joystick\n";
                break;

            case SDL_JOYDEVICEADDED:
            {
                /*const int device = event.jdevice.which;
                joystick_ = SDL_JoystickOpen(device);
                if (joystick_ != nullptr) {
                    SDL_assert(SDL_JoystickFromInstanceID(SDL_JoystickInstanceID(joystick_)) == joystick_);
                }*/
                wichController_ = event.jdevice.which;
                joystick_ = SDL_JoystickOpen(wichController_);
                memset(currentControllerBtnStates, 0, sizeof(Uint8) * static_cast<size_t>(ControllerButton::LENGTH));
                memset(previousControllerBtnStates, 0, sizeof(Uint8) * static_cast<size_t>(ControllerButton::LENGTH));
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
        json inputBindingFile = LoadJson("./../../data/input_binding.json");
        for (int i = 0; i < static_cast<size_t>(InputAction::LENGTH); ++i)
        {
            bindPcInput[inputBindingFile[i]["Actions"]] = static_cast<KeyCode>(inputBindingFile[i]["PcInput"]);
            bindControllerInput[inputBindingFile[i]["Actions"]] = static_cast<ControllerButton>(inputBindingFile[i]["ControllerInput"]);
        }
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
   
    bool InputManager::IsControllerButtonDown(ControllerButton button) const
    {
        const auto buttonIndex = static_cast<int>(button);
        return currentControllerBtnStates[buttonIndex] == 1
            && previousControllerBtnStates[buttonIndex] == 0;
    }

    bool InputManager::IsControllerButtonUp(ControllerButton button) const
    {
        const auto buttonIndex = static_cast<int> (button);
        return currentControllerBtnStates[buttonIndex] == 0
            && previousControllerBtnStates[buttonIndex] == 1;
    }

	bool InputManager::IsControllerButtonHeld(ControllerButton button) const
	{
        const auto buttonIndex = static_cast<int>(button);
        return currentControllerBtnStates[buttonIndex] == 1;
	}


    bool InputManager::IsActionButtonDown(InputAction button) const
    {
        const int actionIndex = static_cast<int>(button);
    	
        if (bindPcInput[actionIndex] < KeyCode::LENGTH 
            || bindControllerInput[actionIndex] < ControllerButton::LENGTH)
         {
            return IsKeyDown(bindPcInput[actionIndex])
        	|| IsControllerButtonDown(bindControllerInput[actionIndex]);
        }
        return false;
    }

    bool InputManager::IsActionButtonUp(InputAction button) const
    {
        const int actionIndex = static_cast<int>(button);

        if (bindPcInput[actionIndex] < KeyCode::LENGTH
            || bindControllerInput[actionIndex] < ControllerButton::LENGTH)
        {
            return IsKeyUp(bindPcInput[actionIndex])
                || IsControllerButtonUp(bindControllerInput[actionIndex]);
        }
        return false;
    }

    bool InputManager::IsActionButtonHeld(InputAction button) const
    {
        const int actionIndex = static_cast<int>(button);

        if (bindPcInput[actionIndex] < KeyCode::LENGTH
            || bindControllerInput[actionIndex] < ControllerButton::LENGTH)
        {
            return IsKeyHeld(bindPcInput[actionIndex])
                || IsControllerButtonHeld(bindControllerInput[actionIndex]);
        }
        return false;
    }


}
