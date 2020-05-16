#pragma once

#include <vector>
#include <mathematics/vector.h>
#include <SDL.h>
#include <json.hpp>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "utilities/service_locator.h"

#define PC_INPUT
#define SWITCH_INPUT


namespace neko::sdl
{
	class SdlEngine;

	/**
		 * \brief enum all the key on an US keyboard
		 */

	enum class KeyCode : uint16_t {
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
		RETURN = SDL_SCANCODE_RETURN,
		ESCAPE = SDL_SCANCODE_ESCAPE,
		BACKSPACE = SDL_SCANCODE_BACKSPACE,
		TAB = SDL_SCANCODE_TAB,
		SPACE = SDL_SCANCODE_SPACE,
		MINUS = SDL_SCANCODE_MINUS,
		EQUALS = SDL_SCANCODE_EQUALS,
		LEFTBRACKET = SDL_SCANCODE_LEFTBRACKET,
		RIGHTBRACKET = SDL_SCANCODE_RIGHTBRACKET,
		BACKSLASH = SDL_SCANCODE_BACKSLASH,
		NONUSHASH = SDL_SCANCODE_NONUSHASH,
		SEMICOLON = SDL_SCANCODE_SEMICOLON,
		APOSTROPHE = SDL_SCANCODE_APOSTROPHE,
		GRAVE = SDL_SCANCODE_GRAVE,
		COMMA = SDL_SCANCODE_COMMA,
		PERIOD = SDL_SCANCODE_PERIOD,
		SLASH = SDL_SCANCODE_SLASH,
		CAPSLOCK = SDL_SCANCODE_CAPSLOCK,
		KEY_F1 = SDL_SCANCODE_F1,
		KEY_F2 = SDL_SCANCODE_F2,
		KEY_F3 = SDL_SCANCODE_F3,
		KEY_F4 = SDL_SCANCODE_F4,
		KEY_F5 = SDL_SCANCODE_F5,
		KEY_F6 = SDL_SCANCODE_F6,
		KEY_F7 = SDL_SCANCODE_F7,
		KEY_F8 = SDL_SCANCODE_F8,
		KEY_F9 = SDL_SCANCODE_F9,
		KEY_F10 = SDL_SCANCODE_F10,
		KEY_F11 = SDL_SCANCODE_F11,
		KEY_F12 = SDL_SCANCODE_F12,
		KEY_PRINTSCREEN = SDL_SCANCODE_PRINTSCREEN,
		KEY_SCROLLLOCK = SDL_SCANCODE_SCROLLLOCK,
		KEY_PAUSE = SDL_SCANCODE_PAUSE,
		KEY_INSERT = SDL_SCANCODE_INSERT,
		KEY_HOME = SDL_SCANCODE_HOME,
		KEY_PAGEUP = SDL_SCANCODE_PAGEUP,
		KEY_DELETE = SDL_SCANCODE_DELETE,
		KEY_END = SDL_SCANCODE_END,
		KEY_PAGEDOWN = SDL_SCANCODE_PAGEDOWN,
		RIGHT = SDL_SCANCODE_RIGHT,
		LEFT = SDL_SCANCODE_LEFT,
		DOWN = SDL_SCANCODE_DOWN,
		UP = SDL_SCANCODE_UP,
		KEY_NUMLOCKCLEAR = SDL_SCANCODE_NUMLOCKCLEAR,
		KEY_KEYPAD_DIVIDE = SDL_SCANCODE_KP_DIVIDE,
		KEY_KEYPAD_MULTIPLY = SDL_SCANCODE_KP_MULTIPLY,
		KEY_KEYPAD_MINUS = SDL_SCANCODE_KP_MINUS,
		KEY_KEYPAD_PLUS = SDL_SCANCODE_KP_PLUS,
		KEY_KEYPAD_ENTER = SDL_SCANCODE_KP_ENTER,
		KEY_KEYPAD_1 = SDL_SCANCODE_KP_1,
		KEY_KEYPAD_2 = SDL_SCANCODE_KP_2,
		KEY_KEYPAD_3 = SDL_SCANCODE_KP_3,
		KEY_KEYPAD_4 = SDL_SCANCODE_KP_4,
		KEY_KEYPAD_5 = SDL_SCANCODE_KP_5,
		KEY_KEYPAD_6 = SDL_SCANCODE_KP_6,
		KEY_KEYPAD_7 = SDL_SCANCODE_KP_7,
		KEY_KEYPAD_8 = SDL_SCANCODE_KP_8,
		KEY_KEYPAD_9 = SDL_SCANCODE_KP_9,
		KEY_KEYPAD_0 = SDL_SCANCODE_KP_0,
		KEY_KEYPAD_PERIOD = SDL_SCANCODE_KP_PERIOD,
		KEY_NONUSBACKSLASH = SDL_SCANCODE_NONUSBACKSLASH,
		KEY_APPLICATION = SDL_SCANCODE_APPLICATION,
		KEY_POWER = SDL_SCANCODE_POWER,
		KEY_KEYPAD_EQUALS = SDL_SCANCODE_KP_EQUALS,
		KEY_F13 = SDL_SCANCODE_F13,
		KEY_F14 = SDL_SCANCODE_F14,
		KEY_F15 = SDL_SCANCODE_F15,
		KEY_F16 = SDL_SCANCODE_F16,
		KEY_F17 = SDL_SCANCODE_F17,
		KEY_F18 = SDL_SCANCODE_F18,
		KEY_F19 = SDL_SCANCODE_F19,
		KEY_F20 = SDL_SCANCODE_F20,
		KEY_F21 = SDL_SCANCODE_F21,
		KEY_F22 = SDL_SCANCODE_F22,
		KEY_F23 = SDL_SCANCODE_F23,
		KEY_F24 = SDL_SCANCODE_F24,
		KEY_HELP = SDL_SCANCODE_HELP,
		KEY_MENU = SDL_SCANCODE_MENU,
		KEY_SELECT = SDL_SCANCODE_SELECT,
		KEY_STOP = SDL_SCANCODE_STOP,
		KEY_AGAIN = SDL_SCANCODE_AGAIN,
		KEY_UNDO = SDL_SCANCODE_UNDO,
		KEY_CUT = SDL_SCANCODE_CUT,
		KEY_COPY = SDL_SCANCODE_COPY,
		KEY_PASTE = SDL_SCANCODE_PASTE,
		KEY_FIND = SDL_SCANCODE_FIND,
		KEY_MUTE = SDL_SCANCODE_MUTE,
		KEY_VOLUMEUP = SDL_SCANCODE_VOLUMEUP,
		KEY_VOLUMEDOWN = SDL_SCANCODE_VOLUMEDOWN,

		// They doesn't seem to exist...!
		/* KEY_LOCKINGCAPSLOCK       = SDL_SCANCODE_LOCKINGCAPSLOCK,
		 KEY_LOCKINGNUMLOCK        = SDL_SCANCODE_LOCKINGNUMLOCK,
		 KEY_LOCKINGSCROLLLOCK     = SDL_SCANCODE_LOCKINGSCROLLLOCK,*/

		 KEY_KEYPAD_COMMA = SDL_SCANCODE_KP_COMMA,
		 KEY_KEYPAD_EQUALSAS400 = SDL_SCANCODE_KP_EQUALSAS400,
		 KEY_INTERNATIONAL1 = SDL_SCANCODE_INTERNATIONAL1,
		 KEY_INTERNATIONAL2 = SDL_SCANCODE_INTERNATIONAL2,
		 KEY_INTERNATIONAL3 = SDL_SCANCODE_INTERNATIONAL3,
		 KEY_INTERNATIONAL4 = SDL_SCANCODE_INTERNATIONAL4,
		 KEY_INTERNATIONAL5 = SDL_SCANCODE_INTERNATIONAL5,
		 KEY_INTERNATIONAL6 = SDL_SCANCODE_INTERNATIONAL6,
		 KEY_INTERNATIONAL7 = SDL_SCANCODE_INTERNATIONAL7,
		 KEY_INTERNATIONAL8 = SDL_SCANCODE_INTERNATIONAL8,
		 KEY_INTERNATIONAL9 = SDL_SCANCODE_INTERNATIONAL9,
		 KEY_LANG1 = SDL_SCANCODE_LANG1,
		 KEY_LANG2 = SDL_SCANCODE_LANG2,
		 KEY_LANG3 = SDL_SCANCODE_LANG3,
		 KEY_LANG4 = SDL_SCANCODE_LANG4,
		 KEY_LANG5 = SDL_SCANCODE_LANG5,
		 KEY_LANG6 = SDL_SCANCODE_LANG6,
		 KEY_LANG7 = SDL_SCANCODE_LANG7,
		 KEY_LANG8 = SDL_SCANCODE_LANG8,
		 KEY_LANG9 = SDL_SCANCODE_LANG9,
		 KEY_ALTERASE = SDL_SCANCODE_ALTERASE,
		 KEY_SYSREQ = SDL_SCANCODE_SYSREQ,
		 KEY_CANCEL = SDL_SCANCODE_CANCEL,
		 KEY_CLEAR = SDL_SCANCODE_CLEAR,
		 KEY_PRIOR = SDL_SCANCODE_PRIOR,
		 KEY_RETURN2 = SDL_SCANCODE_RETURN2,
		 KEY_SEPARATOR = SDL_SCANCODE_SEPARATOR,
		 KEY_OUT = SDL_SCANCODE_OUT,
		 KEY_OPER = SDL_SCANCODE_OPER,
		 KEY_CLEARAGAIN = SDL_SCANCODE_CLEARAGAIN,
		 KEY_CRSEL = SDL_SCANCODE_CRSEL,
		 KEY_EXSEL = SDL_SCANCODE_EXSEL,
		 KEY_KEYPAD_00 = SDL_SCANCODE_KP_00,
		 KEY_KEYPAD_000 = SDL_SCANCODE_KP_000,
		 KEY_THOUSANDSSEPARATOR = SDL_SCANCODE_THOUSANDSSEPARATOR,
		 KEY_DECIMALSEPARATOR = SDL_SCANCODE_DECIMALSEPARATOR,
		 KEY_CURRENCYUNIT = SDL_SCANCODE_CURRENCYUNIT,
		 KEY_CURRENCYSUBUNIT = SDL_SCANCODE_CURRENCYSUBUNIT,
		 KEY_KEYPAD_LEFTPAREN = SDL_SCANCODE_KP_LEFTPAREN,
		 KEY_KEYPAD_RIGHTPAREN = SDL_SCANCODE_KP_RIGHTPAREN,
		 KEY_KEYPAD_LEFTBRACE = SDL_SCANCODE_KP_LEFTBRACE,
		 KEY_KEYPAD_RIGHTBRACE = SDL_SCANCODE_KP_RIGHTBRACE,
		 KEY_KEYPAD_TAB = SDL_SCANCODE_KP_TAB,
		 KEY_KEYPAD_BACKSPACE = SDL_SCANCODE_KP_BACKSPACE,
		 KEY_KEYPAD_A = SDL_SCANCODE_KP_A,
		 KEY_KEYPAD_B = SDL_SCANCODE_KP_B,
		 KEY_KEYPAD_C = SDL_SCANCODE_KP_C,
		 KEY_KEYPAD_D = SDL_SCANCODE_KP_D,
		 KEY_KEYPAD_E = SDL_SCANCODE_KP_E,
		 KEY_KEYPAD_F = SDL_SCANCODE_KP_F,
		 KEY_KEYPAD_XOR = SDL_SCANCODE_KP_XOR,
		 KEY_KEYPAD_POWER = SDL_SCANCODE_KP_POWER,
		 KEY_KEYPAD_PERCENT = SDL_SCANCODE_KP_PERCENT,
		 KEY_KEYPAD_LESS = SDL_SCANCODE_KP_LESS,
		 KEY_KEYPAD_GREATER = SDL_SCANCODE_KP_GREATER,
		 KEY_KEYPAD_AMPERSAND = SDL_SCANCODE_KP_AMPERSAND,
		 KEY_KEYPAD_DBLAMPERSAND = SDL_SCANCODE_KP_DBLAMPERSAND,
		 KEY_KEYPAD_VERTICALBAR = SDL_SCANCODE_KP_VERTICALBAR,
		 KEY_KEYPAD_DBLVERTICALBAR = SDL_SCANCODE_KP_DBLVERTICALBAR,
		 KEY_KEYPAD_COLON = SDL_SCANCODE_KP_COLON,
		 KEY_KEYPAD_HASH = SDL_SCANCODE_KP_HASH,
		 KEY_KEYPAD_SPACE = SDL_SCANCODE_KP_SPACE,
		 KEY_KEYPAD_AT = SDL_SCANCODE_KP_AT,
		 KEY_KEYPAD_EXCLAM = SDL_SCANCODE_KP_EXCLAM,
		 KEY_KEYPAD_MEMSTORE = SDL_SCANCODE_KP_MEMSTORE,
		 KEY_KEYPAD_MEMRECALL = SDL_SCANCODE_KP_MEMRECALL,
		 KEY_KEYPAD_MEMCLEAR = SDL_SCANCODE_KP_MEMCLEAR,
		 KEY_KEYPAD_MEMADD = SDL_SCANCODE_KP_MEMADD,
		 KEY_KEYPAD_MEMSUBTRACT = SDL_SCANCODE_KP_MEMSUBTRACT,
		 KEY_KEYPAD_MEMMULTIPLY = SDL_SCANCODE_KP_MEMMULTIPLY,
		 KEY_KEYPAD_MEMDIVIDE = SDL_SCANCODE_KP_MEMDIVIDE,
		 KEY_KEYPAD_PLUSMINUS = SDL_SCANCODE_KP_PLUSMINUS,
		 KEY_KEYPAD_CLEAR = SDL_SCANCODE_KP_CLEAR,
		 KEY_KEYPAD_CLEARENTRY = SDL_SCANCODE_KP_CLEARENTRY,
		 KEY_KEYPAD_BINARY = SDL_SCANCODE_KP_BINARY,
		 KEY_KEYPAD_OCTAL = SDL_SCANCODE_KP_OCTAL,
		 KEY_KEYPAD_DECIMAL = SDL_SCANCODE_KP_DECIMAL,
		 KEY_KEYPAD_HEXADECIMAL = SDL_SCANCODE_KP_HEXADECIMAL,
		 KEY_LEFT_CTRL = SDL_SCANCODE_LCTRL,
		 KEY_LEFT_SHIFT = SDL_SCANCODE_LSHIFT,
		 KEY_LEFT_ALT = SDL_SCANCODE_LALT,
		 KEY_LEFT_GUI = SDL_SCANCODE_LGUI,
		 KEY_RIGHT_CTRL = SDL_SCANCODE_RCTRL,
		 KEY_RIGHT_SHIFT = SDL_SCANCODE_RSHIFT,
		 KEY_RIGHT_ALT = SDL_SCANCODE_RALT,
		 KEY_RIGHT_GUI = SDL_SCANCODE_RGUI,
		 KEY_MODE = SDL_SCANCODE_MODE,
		 KEY_AUDIONEXT = SDL_SCANCODE_AUDIONEXT,
		 KEY_AUDIOPREV = SDL_SCANCODE_AUDIOPREV,
		 KEY_AUDIOSTOP = SDL_SCANCODE_AUDIOSTOP,
		 KEY_AUDIOPLAY = SDL_SCANCODE_AUDIOPLAY,
		 KEY_AUDIOMUTE = SDL_SCANCODE_AUDIOMUTE,
		 KEY_MEDIASELECT = SDL_SCANCODE_MEDIASELECT,
		 KEY_WWW = SDL_SCANCODE_WWW,
		 KEY_MAIL = SDL_SCANCODE_MAIL,
		 KEY_CALCULATOR = SDL_SCANCODE_CALCULATOR,
		 KEY_COMPUTER = SDL_SCANCODE_COMPUTER,
		 KEY_AC_SEARCH = SDL_SCANCODE_AC_SEARCH,
		 KEY_AC_HOME = SDL_SCANCODE_AC_HOME,
		 KEY_AC_BACK = SDL_SCANCODE_AC_BACK,
		 KEY_AC_FORWARD = SDL_SCANCODE_AC_FORWARD,
		 KEY_AC_STOP = SDL_SCANCODE_AC_STOP,
		 KEY_AC_REFRESH = SDL_SCANCODE_AC_REFRESH,
		 KEY_AC_BOOKMARKS = SDL_SCANCODE_AC_BOOKMARKS,
		 KEY_BRIGHTNESSDOWN = SDL_SCANCODE_BRIGHTNESSDOWN,
		 KEY_BRIGHTNESSUP = SDL_SCANCODE_BRIGHTNESSUP,
		 KEY_DISPLAYSWITCH = SDL_SCANCODE_DISPLAYSWITCH,
		 KEY_KBDILLUMTOGGLE = SDL_SCANCODE_KBDILLUMTOGGLE,
		 KEY_KBDILLUMDOWN = SDL_SCANCODE_KBDILLUMDOWN,
		 KEY_KBDILLUMUP = SDL_SCANCODE_KBDILLUMUP,
		 KEY_EJECT = SDL_SCANCODE_EJECT,
		 KEY_SLEEP = SDL_SCANCODE_SLEEP,
		 KEYBOARD_SIZE = 282
	};

	enum class MouseButtonCode : uint8_t
	{
		LEFT,
		RIGHT,
		MIDDLE,
		LENGTH
	};


	/*
	 * \b enum of the controller button
	 */
	enum class ControllerInputs : uint8_t
	{
		BUTTON_A = 0,
		BUTTON_B = 1,
		BUTTON_X = 2,
		BUTTON_Y = 3,
		LEFT_TRIGGER = 4,
		RIGHT_TRIGGER = 5,
		BUTTON_SELECT = 6,
		BUTTON_START = 7,
		PRESS_LEFT_STICK = 8,
		PRESS_RIGHT_STICK = 9,
		LENGTH
	};

	/*
	 * \b enum of the controller axis
	 */
	enum class ControllerAxis : uint8_t
	{
		HORIZONTAL_LEFT_AXIS = 0,
		VERTICAL_LEFT_AXIS = 1,
		LEFT_BUMPER = 2,
		HORIZONTAL_RIGHT_AXIS = 3,
		VERTICAL_RIGHT_AXIS = 4,
		RIGHT_BUMPER = 5,
		PAD_HORIZONTAL = 6,
		PAD_VERTICAL = 7,
		LENGTH
	};

	/*
	 * \b enum all the button on a switch controller
	 */
	enum class SwitchInputs : uint8_t
	{
		PAD_UP,
		PAD_DOWN,
		PAD_LEFT,
		PAD_RIGHT,
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
		LENGTH
	};

	enum class ButtonState
	{
		NONE,
		DOWN,
		HELD,
		UP,
	};

	enum class InputAction
	{
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3,
		DASH = 4,
		MAIN_SHOOT = 5,
		SECONDARY_TARGET = 6,
		SECONDARY_SHOOT = 7,
		SECONDARY_CANCEL = 8,
		ABILITIES = 9,
		LENGTH = 10
	};


	class IInputManager
	{
	public:

		IInputManager() = default;

		virtual void Init() = 0;

		virtual void BindFromJson() = 0;

		virtual void OnPreUserInput() = 0;

		virtual void ProccesInputs(SDL_Event event) = 0;

		virtual Vec2f GetMousePosition() const = 0;

		virtual Vec2f GetMouseScroll() const = 0;

		virtual float GetJoystickAxis(ControllerAxis axis) const = 0;

		/**
		 * \brief Check if a key input is pressed down
		 */
		virtual bool IsKeyDown(KeyCode key) const = 0;

		/**
		 * \brief Check if a key input is released
		 */
		virtual bool IsKeyUp(KeyCode key) const = 0;

		/**
		 * \brief Check if a key input is held
		 */
		virtual bool IsKeyHeld(KeyCode key) const = 0;

		/**
		 * \brief Check if a mouse button is pressed down
		 */
		virtual bool IsMouseButtonDown(MouseButtonCode button) const = 0;

		/**
		 * \brief Check if a mouse button is released
		 */
		virtual bool IsMouseButtonUp(MouseButtonCode button) const = 0;

		/**
		 * \brief Check if a mouse button is held
		 */
		virtual bool IsMouseButtonHeld(MouseButtonCode button) const = 0;

		/**
		 * \brief Check if a switch input is pressed down
		 */
		virtual bool IsSwitchButtonDown(SwitchInputs key) const = 0;

		/**
		 * \brief Check if a switch input is released
		 */
		virtual bool IsSwitchButtonUp(SwitchInputs key) const = 0;

		/**
		 * \brief Check if a switch input is held
		 */
		virtual bool IsSwitchButtonHeld(SwitchInputs key) const = 0;

		/**
		 * \brief Check if a controller input is pressed down
		 */
		virtual bool IsControllerDown(ControllerInputs key) const = 0;

		/**
		 * \brief Check if a controller input is released
		 */
		virtual bool IsControllerUp(ControllerInputs key) const = 0;

		/**
		 * \brief Check if a controller input is held
		 */
		virtual bool IsControllerHeld(ControllerInputs key) const = 0;

		/**
		 * \brief Check if a bound input is pressed down
		 */
		virtual bool IsActionDown(InputAction button) const = 0;

		/**
		 * \brief Check if a bound input is released
		 */
		virtual bool IsActionUp(InputAction button) const = 0;

		/**
		 * \brief Check if a bound input is held
		 */
		virtual bool IsActionHeld(InputAction button) const = 0;

		virtual void PrintJoystick(const int device) const = 0;

		/**
		* \brief translate ActionCurrent enum to string
		*/
		virtual std::string ActionEnumToString(InputAction action) = 0;
		/**
		* \brief translate KeyCode enum to string
		*/
		virtual std::string PcInputsEnumToString(KeyCode keyCode) = 0;
		virtual std::string PcInputsEnumToString(MouseButtonCode mouseButton) = 0;
		/**
		* \brief translate SwitchInputs enum to string
		*/
		virtual std::string SwitchInputsEnumToString(SwitchInputs switchInputs) = 0;
		/**
		* \brief translate ControllerInputs enum to string
		*/
		virtual std::string ControllerInputsEnumToString(ControllerInputs controller) = 0;

		virtual std::string ControllerAxisEnumToString(const ControllerAxis controller) = 0;


	};
	/**
	 * \brief Manage inputs
	 */;
	 class InputManager final : public IInputManager
	 {
	 public:
		 explicit InputManager(
			 SdlEngine& engine);

		 void Init() override;

		 void BindFromJson() override;

		 void OnPreUserInput() override;

		 void ProccesInputs(SDL_Event event) override;

		 Vec2f GetMousePosition() const override;

		 Vec2f GetMouseScroll() const override;

		 float GetJoystickAxis(ControllerAxis axis) const override;

		 bool IsKeyDown(KeyCode key) const override;

		 bool IsKeyUp(KeyCode key) const override;

		 bool IsKeyHeld(KeyCode key) const override;

		 bool IsSwitchButtonDown(SwitchInputs key) const override;

		 bool IsSwitchButtonUp(SwitchInputs key) const override;

		 bool IsSwitchButtonHeld(SwitchInputs key) const override;

		 bool IsControllerDown(ControllerInputs key) const override;

		 bool IsControllerUp(ControllerInputs key) const override;

		 bool IsControllerHeld(ControllerInputs key) const override;

		 bool IsMouseButtonDown(MouseButtonCode button) const override;

		 bool IsMouseButtonUp(MouseButtonCode button) const override;

		 bool IsMouseButtonHeld(MouseButtonCode button) const override;

		 bool IsActionDown(InputAction button) const override;

		 bool IsActionUp(InputAction button) const override;

		 bool IsActionHeld(InputAction button) const override;

		 void PrintJoystick(int device) const override;

		 std::string ActionEnumToString(InputAction action) override;

		 std::string PcInputsEnumToString(KeyCode keyCode) override;
		 std::string PcInputsEnumToString(MouseButtonCode mouseButton) override;

		 std::string SwitchInputsEnumToString(SwitchInputs switchInputs) override;

		 std::string ControllerInputsEnumToString(ControllerInputs controller) override;
		 std::string ControllerAxisEnumToString(const ControllerAxis controller) override;

	 protected:
		 std::vector<int> bindingPcInput_ =
			 std::vector<int>(static_cast<int>(InputAction::LENGTH));
		 std::vector<int> bindingSwitchInput_ =
			 std::vector<int>(static_cast<int>(InputAction::LENGTH));
		 std::vector<int> bindingControllerInput_ =
			 std::vector<int>(static_cast<int>(InputAction::LENGTH));

		 std::vector<ButtonState> keyPressedState_ =
			 std::vector<ButtonState>(static_cast<int>(KeyCode::KEYBOARD_SIZE));
		 std::vector<ButtonState> buttonState_ =
			 std::vector<ButtonState>(static_cast<int>(MouseButtonCode::LENGTH));
		 std::vector<ButtonState> switchButtonState_ =
			 std::vector<ButtonState>(static_cast<int>(SwitchInputs::LENGTH));
		 std::vector<ButtonState> controllerButtonState_ =
			 std::vector<ButtonState>(static_cast<int>(ControllerInputs::LENGTH));
		 std::vector<float> controllerAxis_ =
			 std::vector<float>(static_cast<int>(ControllerAxis::LENGTH));

		 const uint8_t* keyboard_;
		 uint32_t mouse_;
		 Vec2f mousePos_ = Vec2f::zero;
		 Vec2f mouseScroll_ = Vec2f::zero;

		 SDL_Joystick* joystick_{};
		 SDL_GameController* controller_{};

		 struct ImGuiContext* imGuiContext_ = nullptr;

		 const float kMaxJoyValue_ = 32768.0f;

		 SdlEngine& engine_;
	 };

	 class NullInputManager final : public IInputManager
	 {
	 public:
		 NullInputManager() = default;
		 ~NullInputManager() = default;

		 void Init() override {}

		 void BindFromJson() override {}

		 void OnPreUserInput() {};

		 void ProccesInputs(SDL_Event event) override {}

		 Vec2f GetMousePosition() const override { return Vec2f(0.0f, 0.0f); }

		 Vec2f GetMouseScroll() const override { return Vec2f(0.0f, 0.0f); }

		 float GetJoystickAxis(ControllerAxis axis) const override { return 0.0f; }

		 bool IsKeyDown(KeyCode key) const override
		 {
			 key;
			 return false;
		 }

		 bool IsKeyUp(KeyCode key) const override
		 {
			 key;
			 return false;
		 }

		 bool IsKeyHeld(KeyCode key) const override
		 {
			 key;
			 return false;
		 }


		 bool IsMouseButtonDown(MouseButtonCode button) const override
		 {
			 button;
			 return false;
		 }

		 bool IsMouseButtonUp(MouseButtonCode button) const override
		 {
			 button;
			 return false;
		 }

		 bool IsMouseButtonHeld(MouseButtonCode button) const override
		 {
			 button;
			 return false;
		 }

		 bool IsSwitchButtonDown(SwitchInputs key) const override
		 {
			 key;
			 return false;
		 }

		 bool IsSwitchButtonUp(SwitchInputs key) const override
		 {
			 key;
			 return false;
		 }

		 bool IsSwitchButtonHeld(SwitchInputs key) const override
		 {
			 key;
			 return false;
		 }

		 bool IsControllerDown(ControllerInputs key) const override
		 {
			 key;
			 return false;
		 }

		 bool IsControllerUp(ControllerInputs key) const override
		 {
			 key;
			 return false;
		 }

		 bool IsControllerHeld(ControllerInputs key) const override
		 {
			 key;
			 return false;
		 }

		 bool IsActionDown(InputAction button) const override
		 {
			 button;
			 return false;
		 }

		 bool IsActionUp(InputAction button) const override
		 {
			 button;
			 return false;
		 }

		 bool IsActionHeld(InputAction button) const override
		 {
			 button;
			 return false;
		 }

		 void PrintJoystick(const int device) const override
		 {
			 device;
		 }
		 /**
		 * \brief translate ActionCurrent enum to string
		 */
		 std::string ActionEnumToString(InputAction action) override
		 {
			 action;
			 return "";
		 }
		 /**
		 * \brief translate KeyCode enum to string
		 */
		 std::string PcInputsEnumToString(KeyCode keyCode) override
		 {
			 keyCode;
			 return "";
		 }
		 std::string PcInputsEnumToString(MouseButtonCode mouseButton) override
		 {
			 mouseButton;
			 return "";
		 }
		 /**
		 * \brief translate SwitchInputs enum to string
		 */
		 std::string SwitchInputsEnumToString(SwitchInputs switchInputs) override
		 {
			 switchInputs;
			 return "";
		 }
		 /**
		 * \brief translate ControllerInputs enum to string
		 */
		 std::string ControllerInputsEnumToString(ControllerInputs controller) override
		 {
			 controller;
			 return "";
		 }

		 /**
		 * \brief translate ControllerInputs enum to string
		 */
		 std::string ControllerAxisEnumToString(const ControllerAxis controller) override
		 {
			 controller;
			 return "";
		 }
	 };

	 using InputLocator = Locator<IInputManager, NullInputManager>;
}