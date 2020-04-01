#include <sdl_engine/sdl_input.h>

#include <imgui.h>

#include "utilities/json_utility.h"


namespace neko
{

    void InputManager::Init()
    {
        BindFromJson();

        memset(previousKeyStates_, 0, sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));
        memcpy(currentKeyStates_, SDL_GetKeyboardState(NULL), sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));

        memset(currentControllerBtnStates_, 0, sizeof(Uint8) * static_cast<size_t>(ControllerButton::LENGTH));
        memset(previousControllerBtnStates_, 0, sizeof(Uint8) * static_cast<size_t>(ControllerButton::LENGTH));
        memset(currentControllerAxisValue_, 0, sizeof(float) * static_cast<size_t>(ControllerAxis::LENGTH));
    	
        SDL_JoystickEventState(SDL_ENABLE);

    }

    void InputManager::PreUserInputs()
    {
        memcpy(previousKeyStates_, currentKeyStates_, sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));
        memcpy(currentKeyStates_, SDL_GetKeyboardState(NULL), sizeof(Uint8) * static_cast<size_t>(KeyCode::LENGTH));
    	
        memcpy(&previousControllerBtnStates_, &currentControllerBtnStates_, sizeof(Uint8) * static_cast<size_t>(ControllerButton::LENGTH));

        SDL_GameControllerUpdate();
        const auto controllerLength = static_cast<size_t>(ControllerButton::LENGTH);
        const auto controllerAxisLength = static_cast<size_t>(ControllerAxis::LENGTH);
    	
        for (size_t b = 0; b < controllerLength; ++b) 
        {
            currentControllerBtnStates_[b] = SDL_JoystickGetButton(joystick_, b);
            currentControllerBtnStates_[b] = SDL_JoystickGetHat(joystick_, b);
        }
    	for (size_t a = 0; a < controllerAxisLength; ++a)
            {
				currentControllerAxisValue_[a] = SDL_JoystickGetAxis(joystick_, a);
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
                if (wichController_ = event.cdevice.which)
                {
                    wichController_ = -1;
                    joystick_ = nullptr;
                }
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
                memset(currentControllerBtnStates_, 0, sizeof(Uint8) * static_cast<size_t>(ControllerButton::LENGTH));
                memset(previousControllerBtnStates_, 0, sizeof(Uint8) * static_cast<size_t>(ControllerButton::LENGTH));
                memset(currentControllerAxisValue_, 0, sizeof(float) * static_cast<size_t>(ControllerAxis::LENGTH));
            	
                PrintNumberJoystick(joystick_);
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
            bindPcInput_[inputBindingFile[i]["Actions"]] = static_cast<KeyCode>(inputBindingFile[i]["PcInput"]);
            bindControllerInput_[inputBindingFile[i]["Actions"]] = static_cast<ControllerButton>(inputBindingFile[i]["ControllerInput"]);
        }
    }
	
    bool InputManager::IsKeyDown(KeyCode key) const
    {
        return currentKeyStates_[static_cast<int>(key)] == 1
            && previousKeyStates_[static_cast<int>(key)] == 0;
    }

    bool InputManager::IsKeyUp(KeyCode key) const
    {
        return currentKeyStates_[static_cast<int>(key)] == 0
            && previousKeyStates_[static_cast<int>(key)] == 1;
    }

    bool InputManager::IsKeyHeld(KeyCode key) const
    {
        return currentKeyStates_[static_cast<int>(key)] == 1;
    }
   
    bool InputManager::IsControllerButtonDown(ControllerButton button) const
    {
        const auto buttonIndex = static_cast<int>(button);
        return currentControllerBtnStates_[buttonIndex] == 1
            && previousControllerBtnStates_[buttonIndex] == 0;
    }

    bool InputManager::IsControllerButtonUp(ControllerButton button) const
    {
        const auto buttonIndex = static_cast<int> (button);
        return currentControllerBtnStates_[buttonIndex] == 0
            && previousControllerBtnStates_[buttonIndex] == 1;
    }

	bool InputManager::IsControllerButtonHeld(ControllerButton button) const
	{
        const auto buttonIndex = static_cast<int>(button);
        return currentControllerBtnStates_[buttonIndex] == 1;
	}

	float InputManager::GetAxisValue(ControllerAxis axis) const
	{
        const auto axisValue = static_cast<int>(axis);
        return currentControllerAxisValue_[axisValue];
	}

    bool InputManager::IsActionButtonDown(InputAction button) const
    {
        const int actionIndex = static_cast<int>(button);
    	
        if (bindPcInput_[actionIndex] < KeyCode::LENGTH 
            || bindControllerInput_[actionIndex] < ControllerButton::LENGTH)
         {
            return IsKeyDown(bindPcInput_[actionIndex])
        	|| IsControllerButtonDown(bindControllerInput_[actionIndex]);
        }
        return false;
    }

    bool InputManager::IsActionButtonUp(InputAction button) const
    {
        const int actionIndex = static_cast<int>(button);

        if (bindPcInput_[actionIndex] < KeyCode::LENGTH
            || bindControllerInput_[actionIndex] < ControllerButton::LENGTH)
        {
            return IsKeyUp(bindPcInput_[actionIndex])
                || IsControllerButtonUp(bindControllerInput_[actionIndex]);
        }
        return false;
    }

    bool InputManager::IsActionButtonHeld(InputAction button) const
    {
        const int actionIndex = static_cast<int>(button);

        if (bindPcInput_[actionIndex] < KeyCode::LENGTH
            || bindControllerInput_[actionIndex] < ControllerButton::LENGTH)
        {
            return IsKeyHeld(bindPcInput_[actionIndex])
                || IsControllerButtonHeld(bindControllerInput_[actionIndex]);
        }
        return false;
    }

    void InputManager::PrintNumberJoystick(SDL_Joystick* joystick)
    {
        numberOfJoystickButtons_ = SDL_JoystickNumButtons(joystick);
        numberOfJoystickAxes_ = SDL_JoystickNumAxes(joystick);
        numberOfJoystickBalls_ = SDL_JoystickNumBalls(joystick);
        numberOfJoystickHats_ = SDL_JoystickNumHats(joystick);

        std::cout << "number of buttons : " << numberOfJoystickButtons_ << '\n';
        std::cout << "number of axes : " << numberOfJoystickAxes_ << '\n';
        std::cout << "number of balls : " << numberOfJoystickBalls_ << '\n';
        std::cout << "number of hats : " << numberOfJoystickHats_ << '\n';
    }


}

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30