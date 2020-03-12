#pragma once

#include <vector>
#include <mathematics/vector.h>
#include <SDL.h>
#include <SDL_keycode.h>

typedef SDL_KeyCode KeyCode;

namespace neko
{
	
	enum class ButtonCode : uint8_t
    {
        LEFT,
        RIGHT,
        MIDDLE,
        MOUSE_MAX
    };
    /**
     * \brief Manage inputs
     */
    class InputManager final
	{
    public:
        InputManager::InputManager();

        InputManager::~InputManager();

        void OnPreUserInputs();

        bool IsKeyDown(KeyCode key) const;

        bool IsKeyUp(KeyCode key) const;

        bool IsKeyHeld(KeyCode key) const;
    	
        Vec2f GetMousePosition() const;

        bool IsButtonDown(ButtonCode button) const;

        bool IsButtonUp(ButtonCode button) const;


        inline static int scrollX = 0;
        inline static int scrollY = 0;
        inline static int scrollUpdate = false;

        inline static int joyAxisLX = 0;
        inline static int joyAxisLY = 0;
        inline static int joyAxisRX = 0;
        inline static int joyAxisRY = 0;
        inline static int joyAxisUpdate = false;
    private:

        std::vector<SDL_EventType> keyPressedStates = std::vector<SDL_EventType>(SDL_NUM_SCANCODES);
        std::vector<SDL_EventType> buttonPressedStates = std::vector<SDL_EventType>(static_cast<int>(SDL_LASTEVENT));
        const uint8_t* keyboard_;
        uint32_t mouse_;
        Vec2f mousePos_;
        SDL_Joystick* joystick_{};
	};
}
