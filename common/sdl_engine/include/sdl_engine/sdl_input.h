#pragma once

#include <vector>
#include <mathematics/vector.h>
#include <SDL.h>
#include <json.hpp>

#define PC_INPUT
#define SWITCH_INPUT


namespace neko
{
	/**
	 * \brief enum all the key on an US keyboard
	 */
	
	enum class KeyCode : Uint8
	{
		UNKNOWN = SDL_SCANCODE_UNKNOWN,
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
		L_SHIFT = SDL_SCANCODE_LSHIFT,
		L_CTRL = SDL_SCANCODE_LCTRL,
		L_ALT = SDL_SCANCODE_LALT,
		TAB = SDL_SCANCODE_TAB,
		R_ALT = SDL_SCANCODE_RALT,
		R_CTRL = SDL_SCANCODE_RCTRL,
		R_SHIFT = SDL_SCANCODE_RSHIFT,
		LENGTH = 230
	};
	

	/*
	 * \b enum of the controller button
	 */
	enum class ControllerButton : Uint8
    {
		NONE = SDL_CONTROLLER_BUTTON_INVALID,
		BUTTON_A = SDL_CONTROLLER_BUTTON_A,
		BUTTON_B = SDL_CONTROLLER_BUTTON_B,
		BUTTON_X = SDL_CONTROLLER_BUTTON_X,
		BUTTON_Y = SDL_CONTROLLER_BUTTON_Y,
		BUTTON_SELECT = SDL_CONTROLLER_BUTTON_BACK,
		XBOX_BUTTON = SDL_CONTROLLER_BUTTON_GUIDE,
		BUTTON_START = SDL_CONTROLLER_BUTTON_START,
		PRESS_LEFT_STICK = SDL_CONTROLLER_BUTTON_LEFTSTICK,
		PRESS_RIGHT_STICK = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
		LEFT_BUMPER = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		RIGHT_BUMPER = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
		PAD_UP = SDL_CONTROLLER_BUTTON_DPAD_UP,
		PAD_DOWN = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
		PAD_LEFT = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		PAD_RIGHT = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
		LENGTH //define the array bounds
    };

	enum class ControllerAxis
	{
		HORIZONTAL_LEFT_AXIS = SDL_CONTROLLER_AXIS_LEFTX,//X AXIS
		VERTICAL_LEFT_AXIS = SDL_CONTROLLER_AXIS_LEFTY,//Y AXIS
		VERTICAL_RIGHT_AXIS = SDL_CONTROLLER_AXIS_RIGHTX,//4th axis
		HORIZONTAL_RIGHT_AXIS = SDL_CONTROLLER_AXIS_RIGHTY,//5th axis
		LEFT_TRIGGER = SDL_CONTROLLER_AXIS_TRIGGERLEFT,//9th axis	
		RIGHT_TRIGGER = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,//10th axis
	};
	/*
	 * \b enum all the button on a switch controller
	 */
	enum class SwitchButton
	{
		NONE,
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
	
	enum class InputAction
	{
		NONE = 0,
		UP = 1,
		DOWN = 2,
		LEFT = 3,
		RIGHT = 4,
		LENGTH //define the array bounds
	};
	
	class IInputManager
	{
	public:
		
		IInputManager() = default;

		virtual void Init() = 0;
		
		virtual void PreUserInputs() = 0;
			
		virtual void BindFromJson() = 0;
		
		virtual bool IsKeyDown(KeyCode key) const = 0;

		virtual bool IsKeyUp(KeyCode key) const = 0;

		virtual bool IsKeyHeld(KeyCode key) const = 0;

		virtual bool IsControllerButtonDown(ControllerButton button) const = 0;

		virtual bool IsControllerButtonUp(ControllerButton button) const = 0;

		virtual bool IsControllerButtonHeld(ControllerButton button) const = 0;
		
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

		 void Init() override;
	 	
		 void PreUserInputs() override;
	 	
		 void BindFromJson() override;

		 bool IsKeyDown(KeyCode key) const override;

		 bool IsKeyUp(KeyCode key) const override;

		 bool IsKeyHeld(KeyCode key) const override;

		 bool IsControllerButtonDown(ControllerButton button) const override;

		 bool IsControllerButtonUp(ControllerButton button) const override;

		 bool IsControllerButtonHeld(ControllerButton button) const override;
	 	
		 bool IsActionButtonDown(InputAction button) const override;

		 bool IsActionButtonUp(InputAction button) const override;

		 bool IsActionButtonHeld(InputAction button) const override;


	 private:

		Uint8 previousKeyStates[static_cast<size_t>(KeyCode::LENGTH)] = { static_cast<size_t>(KeyCode::UNKNOWN) };
		Uint8 currentKeyStates[static_cast<size_t>(KeyCode::LENGTH)] = { static_cast<size_t>(KeyCode::UNKNOWN) };

		Uint8 previousControllerBtnStates[static_cast<size_t>(ControllerButton::LENGTH)] = { static_cast<size_t>(ControllerButton::NONE) };
		Uint8 currentControllerBtnStates[static_cast<size_t>(ControllerButton::LENGTH)] = { static_cast<size_t>(ControllerButton::NONE) };
	 	
	 	Uint8 previousSwitchBtnStates[static_cast<size_t>(SwitchButton::LENGTH)] = { static_cast<size_t>(SwitchButton::NONE) };
		Uint8 currentSwitchBtnStates[static_cast<size_t>(SwitchButton::LENGTH)] = { static_cast<size_t>(SwitchButton::NONE) };

		KeyCode bindPcInput[static_cast<size_t>(InputAction::LENGTH)] = { KeyCode::UNKNOWN };
		ControllerButton bindControllerInput[static_cast<size_t>(InputAction::LENGTH)] = { ControllerButton::NONE };

        const uint8_t* states_ = nullptr;
		SDL_GameController* controller_ = nullptr;
        SDL_Joystick* joystick_ = nullptr;
		int wichController_ = -1;
	};
}
