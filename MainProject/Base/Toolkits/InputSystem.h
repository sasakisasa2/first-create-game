#pragma once

#pragma warning(push)
#pragma warning(disable:4201)

#include "Keyboard.h"
#include "Mouse.h"

#include "..\wiGamepad\wiGamePad.h"
#include "..\wiGamepad\wiRawGamepad.h"

struct DXTKKeyboard {
	DirectX::Keyboard::State isPressed{};
	DirectX::Keyboard::KeyboardStateTracker tracker;

	DirectX::Keyboard::State& wasPressedThisFrame  = tracker.pressed;
	DirectX::Keyboard::State& wasReleasedThisFrame = tracker.released;
};

struct DXTKMouse {
	DirectX::Mouse* mouse = nullptr;
	DirectX::Mouse::State isPressed{};
	DirectX::Mouse::ButtonStateTracker was;
	int& scroll = isPressed.scrollWheelValue;

	inline void SetMode(const int mode) const
	{
		//if (!mouse)	return;
		mouse->SetMode((DirectX::Mouse::Mode)mode);
	}

	inline DirectX::XMFLOAT2 GetPosition() const
	{
		DirectX::XMFLOAT2 pos{ (float)isPressed.x, (float)isPressed.y };
		return std::move(pos);
	}

	__declspec(property(get = GetPosition))	DirectX::XMFLOAT2 position;
	__declspec(property(get = GetPosition))	DirectX::XMFLOAT2 delta;
};

namespace DirectXTK {

struct Gamepad {
	Gamepad() : state{}, pressed(0), released(0) {}
	union buttons {
		struct {
			bool Up    : 1;
			bool Left  : 1;
			bool Down  : 1;
			bool Right : 1;
		} DPad;
		struct {
			bool		  : 4;
			bool Button1  : 1;
			bool Button2  : 1;
			bool Button3  : 1;
			bool Button4  : 1;
			bool Button5  : 1;
			bool Button6  : 1;
			bool Button7  : 1;
			bool Button8  : 1;
			bool Button9  : 1;
			bool Button10 : 1;
			bool Button11 : 1;
			bool Button12 : 1;
			bool Button13 : 1;
			bool Button14 : 1;
		};
	};

	union {
		wi::input::ControllerState state;
		buttons isPressed;
	};

	union {
		uint32_t pressed;
		buttons  wasPressedThisFrame;
	};

	union {
		uint32_t released;
		buttons  wasReleasedThisFrame;
	};

	XMFLOAT2&  leftStick = state.thumbstick_L;
	XMFLOAT2& rightStick = state.thumbstick_R;
	float&   leftTrigger = state.trigger_L;
	float&  rightTrigger = state.trigger_R;

	inline bool   isPressedAnyKey() const { return (state.buttons & 0xfffffff0) != 0; }
	inline bool  wasPressedAnyKey() const { return (pressed       & 0xfffffff0) != 0; }
	inline bool wasReleasedAnyKey() const { return (released      & 0xfffffff0) != 0; }

	inline XMFLOAT2 LeftStickVector2D() const
	{
		XMFLOAT2 v{ leftStick.x, -leftStick.y };
		return std::move(v);
	}
	inline XMFLOAT2 RigthStickVector2D() const
	{
		XMFLOAT2 v{ rightStick.x, -rightStick.y };
		return std::move(v);
	}

	inline XMFLOAT2 DpadVector() const
	{
		XMFLOAT2 v{};
		if (isPressed.DPad.Right)	v.x +=  1.0f;
		if (isPressed.DPad.Left )	v.x += -1.0f;
		if (isPressed.DPad.Up   )	v.y +=  1.0f;
		if (isPressed.DPad.Down )	v.y += -1.0f;
		return std::move(v);
	}
	inline XMFLOAT2 DpadVector2D() const
	{
		XMFLOAT2 v{};
		if (isPressed.DPad.Right)	v.x +=  1.0f;
		if (isPressed.DPad.Left )	v.x += -1.0f;
		if (isPressed.DPad.Down )	v.y +=  1.0f;
		if (isPressed.DPad.Up   )	v.y += -1.0f;
		return std::move(v);
	}
};

struct GamepadX {
	GamepadX() : state{}, pressed(0), released(0) {}
	GamepadX(Gamepad&& gamepad)
	{
		state    = gamepad.state;
		pressed  = gamepad.pressed;
		released = gamepad.released;
	}

	union buttons {
		struct {
			bool up    : 1;
			bool left  : 1;
			bool down  : 1;
			bool right : 1;
		} dpad;
		struct {
			bool				  : 4;
			bool xButton		  : 1;
			bool aButton		  : 1;
			bool bButton		  : 1;
			bool yButton		  : 1;
			bool LButton		  : 1;
			bool RButton		  : 1;
			bool leftStickButton  : 1;
			bool rightStickButton : 1;
			bool viewButton		  : 1;
			bool menuButton		  : 1;
		};
	};

	union {
		wi::input::ControllerState state;
		buttons isPressed;
	};

	union {
		uint32_t pressed;
		buttons  wasPressedThisFrame;
	};

	union {
		uint32_t released;
		buttons  wasReleasedThisFrame;
	};

	XMFLOAT2&  leftStick = state.thumbstick_L;
	XMFLOAT2& rightStick = state.thumbstick_R;
	float&   leftTrigger = state.trigger_L;
	float&  rightTrigger = state.trigger_R;

	GamepadX operator=(Gamepad&& gamepad)
	{
		state    = gamepad.state;
		pressed  = gamepad.pressed;
		released = gamepad.released;
		return *this;
	}

	inline bool   isPressedAnyKey() const { return (state.buttons & 0xfffffff0) != 0; }
	inline bool  wasPressedAnyKey() const { return (pressed       & 0xfffffff0) != 0; }
	inline bool wasReleasedAnyKey() const { return (released      & 0xfffffff0) != 0; }

	inline XMFLOAT2 LeftStickVector2D() const
	{
		XMFLOAT2 v{ leftStick.x, -leftStick.y };
		return std::move(v);
	}
	inline XMFLOAT2 RigthStickVector2D() const
	{
		XMFLOAT2 v{ rightStick.x, -rightStick.y };
		return std::move(v);
	}

	inline XMFLOAT2 DpadVector() const
	{
		XMFLOAT2 v{};
		if (isPressed.dpad.right)	v.x +=  1.0f;
		if (isPressed.dpad.left )	v.x += -1.0f;
		if (isPressed.dpad.up   )	v.y +=  1.0f;
		if (isPressed.dpad.down )	v.y += -1.0f;
		return std::move(v);
	}
	inline XMFLOAT2 DpadVector2D() const
	{
		XMFLOAT2 v{};
		if (isPressed.dpad.right)	v.x +=  1.0f;
		if (isPressed.dpad.left )	v.x += -1.0f;
		if (isPressed.dpad.up   )	v.y +=  1.0f;
		if (isPressed.dpad.down )	v.y += -1.0f;
		return std::move(v);
	}
};

struct GamepadPS {
	GamepadPS() : state{}, pressed(0), released(0) {}
	GamepadPS(Gamepad&& gamepad)
	{
		state = gamepad.state;
		pressed = gamepad.pressed;
		released = gamepad.released;
	}

	union buttons {
		struct {
			bool up    : 1;
			bool left  : 1;
			bool down  : 1;
			bool right : 1;
		} DKeys;
		struct {
			bool			    : 4;
			bool squareButton	: 1;
			bool crossButton	: 1;
			bool circleButton	: 1;
			bool triangleButton	: 1;
			bool L1Button		: 1;
			bool R1Button		: 1;
			bool L2Button		: 1;
			bool R2Button		: 1;
			bool shareButton	: 1;
			bool optionsButton	: 1;
			bool L3Button		: 1;
			bool R3Button		: 1;
			bool PSButton		: 1;
			bool touchpadButton	: 1;
		};
	};

	union {
		wi::input::ControllerState state;
		buttons isPressed;
	};

	union {
		uint32_t pressed;
		buttons  wasPressedThisFrame;
	};

	union {
		uint32_t released;
		buttons  wasReleasedThisFrame;
	};

	XMFLOAT2&  leftStick = state.thumbstick_L;
	XMFLOAT2& rightStick = state.thumbstick_R;
	float&   leftTrigger = state.trigger_L;
	float&  rightTrigger = state.trigger_R;

	GamepadPS operator=(Gamepad&& gamepad)
	{
		state    = gamepad.state;
		pressed  = gamepad.pressed;
		released = gamepad.released;
		return *this;
	}

	inline bool   isPressedAnyKey() const { return (state.buttons & 0xfffffff0) != 0; }
	inline bool  wasPressedAnyKey() const { return (pressed       & 0xfffffff0) != 0; }
	inline bool wasReleasedAnyKey() const { return (released      & 0xfffffff0) != 0; }

	inline XMFLOAT2 LeftStickVector2D() const
	{
		XMFLOAT2 v{ leftStick.x, -leftStick.y };
		return std::move(v);
	}
	inline XMFLOAT2 RigthStickVector2D() const
	{
		XMFLOAT2 v{ rightStick.x, -rightStick.y };
		return std::move(v);
	}

	inline XMFLOAT2 DpadVector() const
	{
		XMFLOAT2 v{};
		if (isPressed.DKeys.right)	v.x +=  1.0f;
		if (isPressed.DKeys.left )	v.x += -1.0f;
		if (isPressed.DKeys.up   )	v.y +=  1.0f;
		if (isPressed.DKeys.down )	v.y += -1.0f;
		return std::move(v);
	}
	inline XMFLOAT2 DpadVector2D() const
	{
		XMFLOAT2 v{};
		if (isPressed.DKeys.right)	v.x +=  1.0f;
		if (isPressed.DKeys.left )	v.x += -1.0f;
		if (isPressed.DKeys.down )	v.y +=  1.0f;
		if (isPressed.DKeys.up   )	v.y += -1.0f;
		return std::move(v);
	}
};

struct GamepadSW {
	GamepadSW() : state{}, pressed(0), released(0) {}
	GamepadSW(Gamepad&& gamepad)
	{
		state    = gamepad.state;
		pressed  = gamepad.pressed;
		released = gamepad.released;
	}

	union buttons {
		struct {
			bool up    : 1;
			bool left  : 1;
			bool down  : 1;
			bool right : 1;
		} controlPad;
		struct {
			bool				  : 4;
			bool yButton		  : 1;
			bool bButton		  : 1;
			bool aButton		  : 1;
			bool xButton		  : 1;
			bool LButton		  : 1;
			bool RButton		  : 1;
			bool ZLButton		  : 1;
			bool ZRButton		  : 1;
			bool minusButton	  : 1;
			bool plusButton		  : 1;
			bool leftStickButton  : 1;
			bool rightStickButton : 1;
			bool HOMEButton		  : 1;
			bool captureButton	  : 1;
		};
	};

	union {
		wi::input::ControllerState state;
		buttons isPressed;
	};

	union {
		uint32_t pressed;
		buttons  wasPressedThisFrame;
	};

	union {
		uint32_t released;
		buttons  wasReleasedThisFrame;
	};

	XMFLOAT2&  leftStick = state.thumbstick_L;
	XMFLOAT2& rightStick = state.thumbstick_R;

	GamepadSW operator=(Gamepad&& gamepad)
	{
		state    = gamepad.state;
		pressed  = gamepad.pressed;
		released = gamepad.released;
		return *this;
	}
};

#pragma warning(pop)

enum ButtonState {
	HeldUp,
	HeldDown,
	ReleasedThisFrame,
	PressedThisFrame
};

enum MouseMode {
	Absolute,
	Relative
};

}

class DXTKGamepad {
public:
	inline DirectXTK::Gamepad ElementAtOrDefault(const int player_index) const
	{
		DirectXTK::Gamepad gamepad{};
		if (player_index >= gamepads_.size())
			return std::move(gamepad);

		gamepad.state    = gamepads_[player_index].state;
		gamepad.pressed  =  pressed_[player_index];
		gamepad.released = released_[player_index];

		return std::move(gamepad);
	}

	inline size_t Count() const
	{
		return gamepads_.size();
	}

	//inline bool anyKey(const uint32_t buttons) const { return (buttons & 0xfffffff0) != 0; }

	//inline XMFLOAT2 MakeDpadVector(const uint32_t buttons) const
	//{
	//	XMFLOAT2 v{};
	//	if (buttons & 0x08)	v.x +=  1.0f;
	//	if (buttons & 0x02)	v.x += -1.0f;
	//	if (buttons & 0x01)	v.y +=  1.0f;
	//	if (buttons & 0x04)	v.y += -1.0f;
	//	return std::move(v);
	//}

	inline void SetMotorSpeeds(const int plalyer_index, const float left, const float right) const
	{
		wi::input::ControllerFeedback feedback;
		feedback.vibration_left  = left;
		feedback.vibration_right = right;
		wi::rawgamepad::SetControllerFeedback(feedback, plalyer_index);
	}

	inline void SetLedColor(const int plalyer_index, wi::Color color) const
	{
		wi::input::ControllerFeedback feedback;
		feedback.led_color = color;
		wi::rawgamepad::SetControllerFeedback(feedback, plalyer_index);
	}

private:
	wi::vector<wi::input::Controller>& gamepads_ = wi::gamepad::GetGamepadState();
	wi::vector<uint32_t>&  pressed_ = wi::gamepad::GetGamepadPressed();
	wi::vector<uint32_t>& released_ = wi::gamepad::GetGamepadReleased();
};

struct INPUTSYSTEM {
	DXTKKeyboard Keyboard;
	DXTKMouse    Mouse;
	DXTKGamepad  Gamepad;
};

extern INPUTSYSTEM InputSystem;