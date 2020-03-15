#pragma once

#include <vector>
#include <mathematics/vector.h>
#include <SDL.h>
#include <sdl_engine/sdl_engine.h>

#define PC_INPUT
#define SWITCH_INPUT

namespace neko
{
	/*
	 * \b enum all the key on an US keyboard
	 */
	enum class KeyCode : int
	{
		A = SDL_SCANCODE_A,
		B = SDL_SCANCODE_B,
		C = SDL_SCANCODE_C,
		D = SDL_SCANCODE_D,
		E = SDL_SCANCODE_E,
		F = SDL_SCANCODE_F,
		G = SDL_SCANCODE_G,
		H = SDL_SCANCODE_H,
		I = SDL_SCANCODE_I,
		J = SDL_SCANCODE_J,
		K = SDL_SCANCODE_K,
		L = SDL_SCANCODE_L,
		M = SDL_SCANCODE_M,
		N = SDL_SCANCODE_N,
		O = SDL_SCANCODE_O,
		P = SDL_SCANCODE_P,
		Q = SDL_SCANCODE_Q,
		R = SDL_SCANCODE_R,
		S = SDL_SCANCODE_S,
		T = SDL_SCANCODE_T,
		U = SDL_SCANCODE_U,
		V = SDL_SCANCODE_V,
		W = SDL_SCANCODE_W,
		X = SDL_SCANCODE_X,
		Y = SDL_SCANCODE_Y,
		Z = SDL_SCANCODE_Z,
		KEY_1 = SDL_SCANCODE_1,
		KEY_2 = SDL_SCANCODE_2,
		KEY_3 = SDL_SCANCODE_3,
		KEY_4 = SDL_SCANCODE_4,
		KEY_5 = SDL_SCANCODE_5,
		KEY_6 = SDL_SCANCODE_6,
		KEY_7 = SDL_SCANCODE_7,
		KEY_8 = SDL_SCANCODE_8,
		KEY_9 = SDL_SCANCODE_9,
		KEY_0 = SDL_SCANCODE_0,
		ESCAPE = SDL_SCANCODE_ESCAPE,
		SPACE = SDL_SCANCODE_SPACE,
		ARROW_KEY_RIGHT = SDL_SCANCODE_RIGHT,
		ARROW_KEY_LEFT = SDL_SCANCODE_LEFT,
		ARROW_KEY_DOWN = SDL_SCANCODE_DOWN,
		ARROW_KEY_UP = SDL_SCANCODE_UP,
		LENGTH
	};

	/*
	 * \b enum of the controller button
	 */
	enum class ControllerCode
    {
		BUTTON_A = 0,
		BUTTON_B = 1,
		BUTTON_X = 2,
		BUTTON_Y = 3,
		LEFT_BUMPER = 4,
		RIGHT_BUMPER = 5,
		BUTTON_SELECT = 6,
		BUTTON_START = 7,
		PRESS_LEFT_STICK = 8,
		PRESS_RIGHT_STICK = 9,
		HORIZONTAL_LEFT_AXIS = 10,//X AXIS
		VERTICAL_LEFT_AXIS = 11,//Y AXIS
		VERTICAL_RIGHT_AXIS = 12,//4th axis
		HORIZONTAL_RIGHT_AXIS = 13,//5th axis
		PAD_HORIZONTAL = 14,//6th axis
		PAD_VERTICAL = 15,//7th axis
		LEFT_TRIGGER = 16,//9th axis	
		RIGHT_TRIGGER = 17,//10th axis
		LENGTH //define the array bounds
    };

	/*
	 * \b enum all the button on a switch controller
	 */
	enum class SwitchButton
	{
		PAD_HORIZONTAL,
		PAD_VERTICAL,
		BUTTON_A,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		LEFT_BUMPER,
		RIGHT_BUMPER,
		LEFT_TRIGGER,
		RIGHT_TRIGGER,
		PRESS_LEFT_STICK,
		PRESS_RIGHT_STICK,
		BUTTON_PLUS,
		BUTTON_MINUS,
		LEFT_LEFT_BUMPER,
		LEFT_RIGHT_BUMPER,
		RIGHT_LEFT_BUMPER,
		RIGHT_RIGHT_BUMPER,
		VERTICAL_LEFT_STICK,
		HORIZONTAL_LEFT_STICK,
		VERTICAL_RIGHT_STICK,
		HORIZONTAL_RIGHT_STICK,
		LENGTH //define the array bounds
	};
	
	enum class InputAction {
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
		ACTION_1 = 4,
		ACTION_2 = 5,
		ACTION_3 = 6,
		LENGTH = 7 //define the array bounds
	};
	enum class ButtonState
	{
		NONE,
		UP,
		DOWN,
		HELD
	};
	
	class IInputManager
	{
	public:
		
		IInputManager() = default;
		
		virtual void PreUserInputs() = 0;

		virtual void BindFromJson() = 0;
		
		virtual bool IsKeyDown(KeyCode key) const = 0;

		virtual bool IsKeyUp(KeyCode key) const = 0;

		virtual bool IsKeyHeld(KeyCode key) const = 0;

		virtual bool IsButtonDown(ControllerCode button) const = 0;

		virtual bool IsButtonUp(ControllerCode button) const = 0;

		virtual bool IsActionButtonDown(InputAction button) const = 0;

		virtual bool IsActionButtonUp(InputAction button) const = 0;

		virtual bool IsActionButtonHeld(InputAction button) const = 0;
	};
    /**
     * \brief Manage inputs
     */;
    class InputManager final : public IInputManager
	{
    public:
    	
        void PreUserInputs() override;

		void BindFromJson() override;
    	
        bool IsKeyDown(KeyCode key) const override;

        bool IsKeyUp(KeyCode key) const override;

        bool IsKeyHeld(KeyCode key) const override;
    	
        bool IsButtonDown(ControllerCode button) const override;

        bool IsButtonUp(ControllerCode button) const override;

		bool IsActionButtonDown(InputAction button) const override;
    	
        bool IsActionButtonUp(InputAction button) const override;

        bool IsActionButtonHeld(InputAction button) const override;
    	
    private:

		std::vector<ButtonState> keyPressedStates_ = std::vector<ButtonState>(static_cast<int>(KeyCode::LENGTH));
        std::vector<ButtonState> buttonPressedStates_ = std::vector<ButtonState>(static_cast<int>(ControllerCode::LENGTH));
		std::vector<ButtonState>switchButtonState_ = std::vector<ButtonState>(static_cast<int>(SwitchButton::LENGTH));

		std::vector<int>bindPcInput = std::vector<int>(static_cast<int>(KeyCode::LENGTH));
    	
        const uint8_t* states_;
        uint32_t mouse_;
        Vec2f mousePos_;
        SDL_Joystick* joystick_;
	};
}
