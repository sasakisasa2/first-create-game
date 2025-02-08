#pragma once

#include "wiInput.h"

namespace wi::gamepad
{
	// call once at app start
	void Initialize();

	// call once per frame
	void Update();

	void SetControllerFeedback(const wi::input::ControllerFeedback& data, size_t playerindex = 0);

	wi::vector<wi::input::Controller>& GetGamepadState();
	wi::vector<uint32_t>& GetGamepadPressed();
	wi::vector<uint32_t>& GetGamepadReleased();
}

