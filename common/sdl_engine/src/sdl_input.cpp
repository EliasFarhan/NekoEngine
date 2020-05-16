#include <sdl_engine/sdl_input.h>

#include <imgui.h>

#include "utilities/json_utility.h"
#include "sdl_engine/sdl_engine.h"


namespace neko::sdl
{
    InputManager::InputManager(SdlEngine& engine)
        : keyboard_(nullptr),
        mouse_(0),
        engine_(engine)
    {
    }

    void InputManager::Init()
    {
        imGuiContext_ = ImGui::GetCurrentContext();
        //BindFromJson();
        /* Print information about the joysticks */
        std::cout << "There are " + std::to_string(SDL_NumJoysticks()) + " joysticks attached" << '\n';;
        for (int i = 0; i < SDL_NumJoysticks(); ++i) { PrintJoystick(i); }

        if (SDL_NumJoysticks() > 0) {
            int device = 0;
            joystick_ = SDL_JoystickOpen(device);

            if (joystick_ != NULL) {
                SDL_assert(
                    SDL_JoystickFromInstanceID(SDL_JoystickInstanceID(
                        joystick_)) == joystick_);
            }
        }
    }

    void InputManager::OnPreUserInput()
    {

#ifndef NN_NINTENDO_SDK
        for (size_t i = 0; i < static_cast<int>(KeyCode::KEYBOARD_SIZE); i++) {
            if (keyPressedState_[i] == ButtonState::UP
                ) {
                keyPressedState_[i] = ButtonState::NONE;
            }
            else if (
                keyPressedState_[i] == ButtonState::DOWN) {
                keyPressedState_[i] = ButtonState::HELD;
            }
        }

        for (size_t i = 0; i < static_cast<int>(MouseButtonCode::MOUSE_MAX); i++) {
            if (buttonState_[i] == ButtonState::UP) { buttonState_[i] = ButtonState::NONE; }
            else if (
                buttonState_[i] == ButtonState::DOWN) {
                buttonState_[i] = ButtonState::HELD;
            }
        }

        for (size_t i = 0; i < static_cast<int>(ControllerInputs::LENGTH); i++) {
            if (controllerButtonState_[i] == ButtonState::UP) {
                controllerButtonState_[i] = ButtonState::NONE;
            }
            else if (controllerButtonState_[i] == ButtonState::DOWN) {
                controllerButtonState_[i] = ButtonState::HELD;
            }
        }
#else
        for (size_t i = 0; i < static_cast<int>(SwitchInputs::LENGTH); i++) {
            if (switchButtonState_[i] == ButtonState::UP)
            {
                switchButtonState_[i] = ButtonState::NONE;
            }
            else if (switchButtonState_[i] == ButtonState::DOWN) {
                switchButtonState_[i] = ButtonState::HELD;
            }
        }
#endif

        /*LogDebug(
            std::to_string(joyAxisLX) + " ; " +
            std::to_string(joyAxisLY) + " ; " +
            std::to_string(joyAxisRX) + " ; " +
            std::to_string(joyAxisRY), LogType::PHYSICS_LOG);*/

    }

    void InputManager::ProccesInputs(SDL_Event event)
    {
        switch (event.type) {
        case SDL_QUIT:
            engine_.Destroy();
            break;

#pragma region keyboard
        case SDL_TEXTINPUT: {
            //TODO(@Nico) ajouter le text input
            //usingController = false;
            break;
        }

        case SDL_KEYDOWN: {
            this->keyboard_ = SDL_GetKeyboardState(nullptr);

            const int index = event.key.keysym.scancode;
            keyPressedState_[index] = ButtonState::DOWN;
            //usingController = false;
            break;
        }
        case SDL_KEYUP: {
            this->keyboard_ = SDL_GetKeyboardState(nullptr);

            const int index = event.key.keysym.scancode;
            keyPressedState_[index] = ButtonState::UP;
            //usingController = false;
            break;
        }
#pragma endregion

#pragma region Mouse
        case SDL_MOUSEMOTION: {
            mousePos_.x = -event.motion.x;
            mousePos_.y = -event.motion.y;
            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            this->mouse_ = SDL_GetMouseState(nullptr, nullptr);

            if (event.button.button == SDL_BUTTON_LEFT)
                buttonState_[static_cast<int>(MouseButtonCode::LEFT)] = ButtonState::DOWN;

            else if (event.button.button == SDL_BUTTON_RIGHT)
                buttonState_[static_cast<int>(MouseButtonCode::RIGHT)] = ButtonState::DOWN;

            else if (event.button.button == SDL_BUTTON_MIDDLE)
                buttonState_[static_cast<int>(MouseButtonCode::MIDDLE)] = ButtonState::DOWN;

            //usingController = false;
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            this->mouse_ = SDL_GetMouseState(nullptr, nullptr);

            if (event.button.button == SDL_BUTTON_LEFT)
                buttonState_[static_cast<int>(MouseButtonCode::LEFT)] = ButtonState::UP;

            else if (event.button.button == SDL_BUTTON_RIGHT)
                buttonState_[static_cast<int>(MouseButtonCode::RIGHT)] = ButtonState::UP;

            else if (event.button.button == SDL_BUTTON_MIDDLE)
                buttonState_[static_cast<int>(MouseButtonCode::MIDDLE)] = ButtonState::UP;

            //usingController = false;
            break;
        }
        case SDL_MOUSEWHEEL: {/*
            scrollX = event.wheel.y;
            scrollY = event.wheel.y;

            scrollUpdate = true;

            usingController = false;*/
            break;
        }
#pragma endregion

#pragma region Switch
        case SDL_JOYDEVICEREMOVED: {
            std::cout << "Joystick device  removed." + std::to_string(static_cast<int>(event.jdevice.which)) << '\n';
            std::cout << "Our instance ID is" + std::to_string(
                    static_cast<int>(SDL_JoystickInstanceID(joystick_))) << '\n';

            SDL_JoystickClose(joystick_);

            std::cout << "No more joystick\n" << '\n';

            //usingController = false;
            break;
        }
        case SDL_JOYDEVICEADDED: {
            const int device = event.jdevice.which;
            joystick_ = SDL_JoystickOpen(device);
            if (joystick_ != nullptr) {
                SDL_assert(
                    SDL_JoystickFromInstanceID(SDL_JoystickInstanceID
                    (joystick_)) == joystick_);
            }
            break;
        }
        case SDL_JOYBALLMOTION: {
            std::cout << "Joystick " + std::to_string(event.jball.which) + " ball " + std::
                to_string(event.jball.ball) + " delta: (" + std::to_string(event.jball.xrel) + "," +
                std::to_string(event.jball.yrel) + ")" << '\n';
            break;
        }
        case SDL_JOYBUTTONDOWN: {
            std::cout <<
                "Joystick " << std::to_string(event.jbutton.which) << " button " + std::to_string(
                    event.jbutton.button) << " down\n";

#ifndef NN_NINTENDO_SDK
            controllerButtonState_[event.jbutton.button] = ButtonState::DOWN;
#else
            switchButtonState_[event.jbutton.button] = ButtonState::DOWN;
#endif

            //usingController = true;
            break;
        }
        case SDL_JOYAXISMOTION: {
            //LogDebug("Joystick Motion !");
            //LogDebug(
            //	"Joystick" + std::to_string(event.jaxis.which) + "axis" + std::to_string(
            //		event.jaxis.axis) + "value:" + std::to_string(event.jaxis.value), LogType::INPUT_LOG);

            // Considering 32540 is the max value, we take a 10% deadZone
            const int deadZone = 3200;
            float value = 0;
            if (event.jaxis.value < -deadZone || event.jaxis.value > deadZone) {
                value = event.jaxis.value / kMaxJoyValue_;
                //usingController = true;
            }

            //switch (event.jaxis.axis) {

            //case 0:
            //    joyAxisLX = value;
            //    break;
            //case 1:
            //    joyAxisLY = value;
            //    break;
            //case 2:
            //    joyAxisRX = value;
            //    break;
            //case 3:
            //    joyAxisRY = value;
            //    break;
            //}
            //joyAxisUpdate = true;
        }
                              break;
        case SDL_JOYHATMOTION: {
            std::string s = "Joystick " + std::to_string(event.jhat.which) +
                " hat " + std::to_string(event.jhat.hat) +
                " value:";/*
            if (event.jhat.value == SDL_HAT_CENTERED) {
                joyAxisLX = 0.0f;
                joyAxisLY = 0.0f;
                s += " centered";
            }
            if (event.jhat.value & SDL_HAT_UP) {
                joyAxisLY = -1.0f;
                s += " up";
            }
            if (event.jhat.value & SDL_HAT_RIGHT) {
                joyAxisLX = 1.0f;
                s += " right";
            }
            if (event.jhat.value & SDL_HAT_DOWN) {
                joyAxisLY = 1.0f;
                s += " down";
            }
            if (event.jhat.value & SDL_HAT_LEFT) {
                joyAxisLX = -1.0f;
                s += " left";
            }

            usingController = true;
            LogDebug(s, LogType::INPUT_LOG);*/
        }
                             break;

        case SDL_JOYBUTTONUP: {
            std::cout << "Joystick " + std::to_string(event.jbutton.which) + " button " + std::
                to_string(event.jbutton.button) + " up\n" << '\n';
#ifndef NN_NINTENDO_SDK
            controllerButtonState_[event.jbutton.button] = ButtonState::UP;
#else
            switchButtonState_[event.jbutton.button] = ButtonState::UP;
#endif
            //usingController = true;
            break;
        }
                            /* Fall through to signal quit */
        case SDL_FINGERDOWN: {

            std::cout << "Finger down" << '\n';
            break;
        }
        case SDL_FINGERMOTION: {
            std::cout << "Finger motion" << '\n';
            break;
        }
        case SDL_FINGERUP: {
            std::cout << "Finger up" << '\n';
            break;
        }
#pragma endregion

#pragma region Controller
        case SDL_CONTROLLERAXISMOTION: {
            std::cout << "Controller" + std::to_string(event.caxis.which) + "axis" + std::to_string(
                    event.caxis.axis) + "value:" + std::to_string(event.caxis.value) << '\n';
            //usingController = true;
            break;
        }
        case SDL_CONTROLLERBUTTONDOWN: {
            std::cout << "Controller " + std::to_string(event.cbutton.which) + " button " + std::to_string(
                    event.cbutton.button) + " up" << '\n';
            controllerButtonState_[event.cbutton.button] = ButtonState::DOWN;

            //usingController = true;
            break;
        }
        case SDL_CONTROLLERBUTTONUP: {
            std::cout << "Controller " + std::to_string(event.cbutton.which) + " button " + std::to_string(
                    event.cbutton.button) + " up" << '\n';
            controllerButtonState_[event.cbutton.button] = ButtonState::UP;
            break;
        }
        case SDL_CONTROLLERDEVICEADDED: {
            const int device = event.cdevice.which;
            controller_ = SDL_GameControllerOpen(device);
            std::cout << "Controller device  added." + std::to_string(
                    static_cast<int>(event.cdevice.which)) << '\n';
            break;
        }
        case SDL_CONTROLLERDEVICEREMOVED: {
            std::cout << "Controller device removed." + std::to_string(
                    static_cast<int>(event.cdevice.which)) << '\n';
            SDL_GameControllerClose(controller_);
            break;
        }
        case SDL_CONTROLLERDEVICEREMAPPED: {
            std::cout << "Controller device remapped." + std::to_string(
                    static_cast<int>(event.cdevice.which)) << '\n';
            break;
        }
#pragma endregion
        default:
            break;
        }
    }

    //void InputManager::BindFromJson()
    //{
    //    json inputBindingFile = LoadJson("./../../data/input_binding.json");
    //    for (int i = 0; i < static_cast<size_t>(InputAction::LENGTH); ++i)
    //    {
    //        bindingPcInput_[inputBindingFile[i]["Actions"]] = static_cast<KeyCode>(inputBindingFile[i]["PcInput"]);
    //        bindingControllerInput_[inputBindingFile[i]["Actions"]] = static_cast<C>(inputBindingFile[i]["ControllerInput"]);
    //    }
    //}

    bool InputManager::IsKeyDown(KeyCode key) const
    {
        return keyPressedState_[static_cast<int>(key)] == ButtonState::DOWN;
    }

    bool InputManager::IsKeyUp(KeyCode key) const
    {
        return keyPressedState_[static_cast<int>(key)] == ButtonState::UP;
    }

    bool InputManager::IsKeyHeld(KeyCode key) const
    {
        if (!keyboard_ || keyboard_ == nullptr)
            return false;

        const auto sdlKey = static_cast<int>(key);

        return keyboard_[sdlKey] != 0;
    }

    bool InputManager::IsSwitchButtonDown(SwitchInputs key) const
    {
        return switchButtonState_[static_cast<int>(key)] == ButtonState::DOWN;
    }

    bool InputManager::IsSwitchButtonUp(SwitchInputs key) const
    {
        return switchButtonState_[static_cast<int>(key)] == ButtonState::UP;
    }

    bool InputManager::IsSwitchButtonHeld(SwitchInputs key) const
    {
        return switchButtonState_[static_cast<int>(key)] == ButtonState::HELD;
    }

    bool InputManager::IsControllerDown(ControllerInputs key) const
    {
        return controllerButtonState_[static_cast<int>(key)] == ButtonState::DOWN;
    }

    bool InputManager::IsControllerUp(ControllerInputs key) const
    {
        return controllerButtonState_[static_cast<int>(key)] == ButtonState::UP;
    }

    bool InputManager::IsControllerHeld(ControllerInputs key) const
    {
        return controllerButtonState_[static_cast<int>(key)] == ButtonState::HELD;
    }

    Vec2f InputManager::GetMousePosition() const { return mousePos_; }

    bool InputManager::IsMouseButtonDown(MouseButtonCode button) const
    {
        return buttonState_[static_cast<int>(button)] == ButtonState::DOWN;
    }

    bool InputManager::IsMouseButtonUp(MouseButtonCode button) const
    {
        return buttonState_[static_cast<int>(button)] == ButtonState::UP;
    }

    bool InputManager::IsMouseButtonHeld(const MouseButtonCode button) const
    {
        switch (button) {
        case MouseButtonCode::LEFT:
            if (mouse_ & SDL_BUTTON(1)) {
                return true;
            }
            break;
        case MouseButtonCode::RIGHT:
            if (mouse_ & SDL_BUTTON(3)) {
                return true;
            }
            break;
        case MouseButtonCode::MIDDLE:
            if (mouse_ & SDL_BUTTON(2)) {
                return true;
            }
            break;
        default:
            break;
        }

        return false;
    }

    bool InputManager::IsActionDown(InputAction button) const
    {
        const int actionIndex = static_cast<int>(button);
#ifndef NN_NINTENDO_SDK
        if (bindingPcInput_[actionIndex] < static_cast<int>(KeyCode::KEYBOARD_SIZE)) {
            return IsKeyDown(static_cast<KeyCode>(bindingPcInput_[actionIndex])) ||
                IsControllerDown(static_cast<ControllerInputs>(
                    bindingControllerInput_[actionIndex])
                );
        }
        return IsMouseButtonDown(
            static_cast<MouseButtonCode>(
                bindingPcInput_[actionIndex] - static_cast<int>(KeyCode::KEYBOARD_SIZE))) ||
            IsControllerDown(static_cast<ControllerInputs>(bindingControllerInput_[actionIndex])
            );
#else
        return IsSwitchButtonDown(static_cast<SwitchInputs>(bindingSwitchInput_[actionIndex]));
#endif
        return false;
    }

    bool InputManager::IsActionUp(InputAction button) const
    {
        const int actionIndex = static_cast<int>(button);
#ifndef NN_NINTENDO_SDK
        if (bindingPcInput_[actionIndex] < static_cast<int>(KeyCode::KEYBOARD_SIZE)) {
            return IsKeyUp(static_cast<KeyCode>(bindingPcInput_[actionIndex])) | IsControllerUp(
                static_cast<ControllerInputs>(bindingControllerInput_[actionIndex]));
        }

        return IsMouseButtonUp(
            static_cast<MouseButtonCode>(
                bindingPcInput_[actionIndex] - static_cast<int>(KeyCode::KEYBOARD_SIZE))) |
            IsControllerUp(static_cast<ControllerInputs>(bindingControllerInput_[actionIndex]));
#else
        return IsSwitchButtonUp(static_cast<SwitchInputs>(bindingSwitchInput_[actionIndex]));
#endif
        return false;
    }

    bool InputManager::IsActionHeld(InputAction button) const
    {
        const int actionIndex = static_cast<int>(button);
#ifndef NN_NINTENDO_SDK
        if (bindingPcInput_[actionIndex] < static_cast<int>(KeyCode::KEYBOARD_SIZE)) {
            return IsKeyHeld(static_cast<KeyCode>(bindingPcInput_[actionIndex])) ||
                IsControllerHeld(static_cast<ControllerInputs>(bindingControllerInput_[actionIndex]));
        }

        return IsMouseButtonHeld(
            static_cast<MouseButtonCode>(bindingPcInput_[actionIndex] - static_cast<int>(KeyCode::KEYBOARD_SIZE))) ||
            IsControllerHeld(static_cast<ControllerInputs>(bindingControllerInput_[actionIndex])
            );

#else
        return IsSwitchButtonHeld(static_cast<SwitchInputs>(bindingSwitchInput_[actionIndex]));
#endif
        return false;
    }

    void InputManager::PrintJoystick(const int device) const
    {
        //Print info
        const char* name = SDL_JoystickNameForIndex(device);

        std::cout << "Joystick " + std::to_string(device) + ": " + name + " Unknown Joystick" << '\n';

        const auto joystick = SDL_JoystickOpen(device);
        if (joystick == nullptr) {
            std::cout << "SDL_JoystickOpen " + std::to_string(device) + " failed: %s\n" << '\n';
        }
        else {
            const char* type;

            char guid[64];
            SDL_assert(
                SDL_JoystickFromInstanceID(SDL_JoystickInstanceID(joystick))
                == joystick);
            SDL_JoystickGetGUIDString(
                SDL_JoystickGetGUID(joystick),
                guid,
                sizeof(guid));
            switch (SDL_JoystickGetType(joystick)) {
            case SDL_JOYSTICK_TYPE_GAMECONTROLLER:
                type = "Game Controller";
                break;
            case SDL_JOYSTICK_TYPE_WHEEL:
                type = "Wheel";
                break;
            case SDL_JOYSTICK_TYPE_ARCADE_STICK:
                type = "Arcade Stick";
                break;
            case SDL_JOYSTICK_TYPE_FLIGHT_STICK:
                type = "Flight Stick";
                break;
            case SDL_JOYSTICK_TYPE_DANCE_PAD:
                type = "Dance Pad";
                break;
            case SDL_JOYSTICK_TYPE_GUITAR:
                type = "Guitar";
                break;
            case SDL_JOYSTICK_TYPE_DRUM_KIT:
                type = "Drum Kit";
                break;
            case SDL_JOYSTICK_TYPE_ARCADE_PAD:
                type = "Arcade Pad";
                break;
            case SDL_JOYSTICK_TYPE_THROTTLE:
                type = "Throttle";
                break;
            default:
                type = "Unknown";
                break;
            }
            // TODO (@Nico) Ain't compile on Switch|Release. Must be a problem about string calculation.
            //LogDebug("       type: " + *type, LogType::INPUT_LOG);
            //LogDebug("       axes: " + SDL_JoystickNumAxes(joystick), LogType::INPUT_LOG);
            //LogDebug("      balls: " + SDL_JoystickNumBalls(joystick), LogType::INPUT_LOG);
            //LogDebug("       hats: " + SDL_JoystickNumHats(joystick), LogType::INPUT_LOG);
            //LogDebug("    buttons: " + SDL_JoystickNumButtons(joystick), LogType::INPUT_LOG);
            //LogDebug("instance id: " + SDL_JoystickInstanceID(joystick), LogType::INPUT_LOG);
            //LogDebug("       guid: " + *guid, LogType::INPUT_LOG);
            //LogDebug("    VID/PID: " + std::to_string(static_cast<int>(SDL_JoystickGetVendor(joystick))) + ":" + std::to_string(static_cast<int>(SDL_JoystickGetProduct(joystick))), LogType::INPUT_LOG);

            SDL_JoystickClose(joystick);
        }
    }

    std::string InputManager::ActionEnumToString(const InputAction action)
    {
        switch (action) {
        case InputAction::UP: return "Up";
        case InputAction::DOWN: return "Down";
        case InputAction::LEFT: return "Left";
        case InputAction::RIGHT: return "Right";
        case InputAction::DASH: return "Dash";
        case InputAction::MAIN_SHOOT: return "Main_Shoot";
        case InputAction::SECONDARY_TARGET: return "Secondary_Target";
        case InputAction::SECONDARY_SHOOT: return "Secondary_Shoot";
        case InputAction::SECONDARY_CANCEL: return "Secondary_cancel";
        case InputAction::ABILITIES: return "Abilities";
        default: return "";
        }
    }

    std::string InputManager::PcInputsEnumToString(const KeyCode keyCode)
    {
        switch (keyCode) {
        case KeyCode::A: return "A";
        case KeyCode::B: return "B";
        case KeyCode::C: return "C";
        case KeyCode::D: return "D";
        case KeyCode::E: return "E";
        case KeyCode::F: return "F";
        case KeyCode::G: return "G";
        case KeyCode::H: return "H";
        case KeyCode::I: return "I";
        case KeyCode::J: return "J";
        case KeyCode::K: return "K";
        case KeyCode::L: return "L";
        case KeyCode::M: return "M";
        case KeyCode::N: return "N";
        case KeyCode::O: return "O";
        case KeyCode::P: return "P";
        case KeyCode::Q: return "Q";
        case KeyCode::R: return "R";
        case KeyCode::S: return "S";
        case KeyCode::T: return "T";
        case KeyCode::U: return "U";
        case KeyCode::V: return "V";
        case KeyCode::W: return "W";
        case KeyCode::X: return "X";
        case KeyCode::Y: return "Y";
        case KeyCode::Z: return "Z";
        case KeyCode::ESCAPE: return "Escape";
        case KeyCode::SPACE: return "Space";
        case KeyCode::KEY_LEFT_SHIFT: return "Left_Shift";
        case KeyCode::KEY_RIGHT_SHIFT: return "Right_Shift";
        case KeyCode::KEY_LEFT_CTRL: return "Left_Ctrl";
        case KeyCode::KEY_RIGHT_CTRL: return "Right_Ctrl";
        case KeyCode::KEY_LEFT_ALT: return "Left_Alt";
        default:  return "";
        }
    }

    std::string InputManager::PcInputsEnumToString(MouseButtonCode mouseButton)
    {
        switch (mouseButton) {
        case MouseButtonCode::LEFT: return "LeftMouse";
        case MouseButtonCode::RIGHT: return "RightMouse";
        case MouseButtonCode::MIDDLE: return "MiddleMouse";
        case MouseButtonCode::MOUSE_MAX: return "MouseMax";
        default:
            return "";
        }
    }

    std::string InputManager::SwitchInputsEnumToString(const SwitchInputs switchInputs)
    {
        switch (switchInputs) {
        case SwitchInputs::PAD_UP: return "Pad_Up";
        case SwitchInputs::PAD_DOWN: return "Pad_Down";
        case SwitchInputs::PAD_LEFT: return "Pad_Left";
        case SwitchInputs::PAD_RIGHT: return "Pad_Right";
        case SwitchInputs::BUTTON_A: return "Button_A";
        case SwitchInputs::BUTTON_B: return "Button_B";
        case SwitchInputs::BUTTON_X: return "Button_X";
        case SwitchInputs::BUTTON_Y: return "Button_Y";
        case SwitchInputs::LEFT_BUMPER: return "Left_Bumper";
        case SwitchInputs::RIGHT_BUMPER: return "Right_Bumper";
        case SwitchInputs::LEFT_TRIGGER: return "Left_Trigger";
        case SwitchInputs::RIGHT_TRIGGER: return "Right_Trigger";
        case SwitchInputs::PRESS_LEFT_STICK: return "Press_Left_Stick";
        case SwitchInputs::PRESS_RIGHT_STICK: return "Press_Right_Stick";
        case SwitchInputs::BUTTON_PLUS: return "Button_Plus";
        case SwitchInputs::BUTTON_MINUS: return "Button_Minus";
        case SwitchInputs::LEFT_LEFT_BUMPER: return "Left_Left_Bumper";
        case SwitchInputs::LEFT_RIGHT_BUMPER: return "Left_Right_Bumper";
        case SwitchInputs::RIGHT_LEFT_BUMPER: return "Right_Left_Bumper";
        case SwitchInputs::RIGHT_RIGHT_BUMPER: return "Right_Right_Bumper";
        case SwitchInputs::VERTICAL_LEFT_STICK: return "Vertical_Left_Stick";
        case SwitchInputs::HORIZONTAL_LEFT_STICK: return "Horizontal_Left_Stick";
        case SwitchInputs::VERTICAL_RIGHT_STICK: return "Vertical_Right_Stick";
        case SwitchInputs::HORIZONTAL_RIGHT_STICK: return "Horizontal_Right_Stick";
        default: return "";
        }
    }

    std::string InputManager::ControllerInputsEnumToString(const ControllerInputs controller)
    {
        switch (controller) {
        case ControllerInputs::VERTICAL_LEFT_AXIS: return "Vertical_Left_Stick";
        case ControllerInputs::HORIZONTAL_LEFT_AXIS: return "Horizontal_Left_Stick";
        case ControllerInputs::PRESS_LEFT_STICK: return "Press_Left_Stick";
        case ControllerInputs::LEFT_BUMPER: return "Left_Bumper";
        case ControllerInputs::LEFT_TRIGGER: return "Left_Trigger";
        case ControllerInputs::VERTICAL_RIGHT_AXIS: return "Vertical_Right_stick";
        case ControllerInputs::HORIZONTAL_RIGHT_AXIS: return "Horizontal_Right_Stick";
        case ControllerInputs::PRESS_RIGHT_STICK: return "Press_Right_Click";
        case ControllerInputs::RIGHT_BUMPER: return "Right_Bumper";
        case ControllerInputs::RIGHT_TRIGGER: return "Right_Trigger";
        case ControllerInputs::BUTTON_A: return "Button_A";
        case ControllerInputs::BUTTON_B: return "Button_B";
        case ControllerInputs::BUTTON_X: return "Button_X";
        case ControllerInputs::BUTTON_Y: return "Button_Y";
        case ControllerInputs::PAD_HORIZONTAL: return "Pad_Horizontal";
        case ControllerInputs::PAD_VERTICAL: return "Pad_Vertical";
        case ControllerInputs::BUTTON_START: return "Button_Start";
        case ControllerInputs::BUTTON_SELECT: return "Button_Select";
        default: return "";
        }
    }




}