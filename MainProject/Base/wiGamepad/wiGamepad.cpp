#include "wiGamePad.h"
#include "wiRawGamepad.h"
#include "wiXInput.h"

#include <algorithm>
#include <atomic>

namespace wi::input {
	wi::vector<Controller> controllers;
}

namespace wi::gamepad
{
	wi::vector<uint32_t> pressed;
	wi::vector<uint32_t> released;

	void Initialize()
	{
		wi::rawgamepad::Initialize();
	}

	void Update()
	{
		wi::input::xinput::Update();
		wi::rawgamepad::Update();

		// Check if low-level XINPUT controller is not registered for playerindex slot and register:
		for (int i = 0; i < wi::input::xinput::GetMaxControllerCount(); ++i)
		{
			if (wi::input::xinput::GetControllerState(nullptr, i))
			{
				int slot = -1;
				for (size_t j = 0; j < (int)wi::input::controllers.size(); ++j)
				{
					if (slot < 0 && wi::input::controllers[j].deviceType == wi::input::Controller::DISCONNECTED)
					{
						// take the first disconnected slot
						slot = (int)j;
					}
					if (wi::input::controllers[j].deviceType == wi::input::Controller::XINPUT && wi::input::controllers[j].deviceIndex == i)
					{
						// it is already registered to this slot
						slot = (int)j;
						break;
					}
				}
				if (slot == -1)
				{
					// no disconnected slot was found, and it was not registered
					slot = (int)wi::input::controllers.size();
					wi::input::controllers.emplace_back();

					pressed .emplace_back();
					released.emplace_back();
				}
				wi::input::controllers[slot].deviceType  = wi::input::Controller::XINPUT;
				wi::input::controllers[slot].deviceIndex = i;
			}
		}

		// Check if low-level RAWINPUT controller is not registered for playerindex slot and register:
		for (int i = 0; i < wi::rawgamepad::GetMaxControllerCount(); ++i)
		{
			if (wi::rawgamepad::GetControllerState(nullptr, i))
			{
				int slot = -1;
				for (int j = 0; j < (int)wi::input::controllers.size(); ++j)
				{
					if (slot < 0 && wi::input::controllers[j].deviceType == wi::input::Controller::DISCONNECTED)
					{
						// take the first disconnected slot
						slot = j;
					}
					if (wi::input::controllers[j].deviceType == wi::input::Controller::RAWINPUT && wi::input::controllers[j].deviceIndex == i)
					{
						// it is already registered to this slot
						slot = j;
						break;
					}
				}
				if (slot == -1)
				{
					// no disconnected slot was found, and it was not registered
					slot = (int)wi::input::controllers.size();
					wi::input::controllers.emplace_back();

					pressed .emplace_back();
					released.emplace_back();
				}
				wi::input::controllers[slot].deviceType = wi::input::Controller::RAWINPUT;
				wi::input::controllers[slot].deviceIndex = i;
			}
		}

		// Read low-level controllers:
		for (int i = 0; i < wi::input::controllers.size(); ++i)
		{
			const uint32_t state = wi::input::controllers[i].state.buttons;
			bool connected = false;
			switch (wi::input::controllers[i].deviceType)
			{
			case wi::input::Controller::XINPUT:
				connected = wi::input::xinput::GetControllerState(&wi::input::controllers[i].state, wi::input::controllers[i].deviceIndex);
				break;
			case wi::input::Controller::RAWINPUT:
				connected = wi::rawgamepad::GetControllerState(&wi::input::controllers[i].state, wi::input::controllers[i].deviceIndex);
				break;
			}

			if (connected)
			{
				pressed [i] = ~state &  wi::input::controllers[i].state.buttons;
				released[i] =  state & ~wi::input::controllers[i].state.buttons;
			}
			else
			{
				wi::input::controllers[i].deviceType = wi::input::Controller::DISCONNECTED;
				pressed [i] = 0;
				released[i] = 0;
			}
		}
	}

	void SetControllerFeedback(const wi::input::ControllerFeedback& data, size_t playerindex)
	{
		if (playerindex < (int)wi::input::controllers.size())
		{
			const wi::input::Controller& controller = wi::input::controllers[playerindex];

			if (controller.deviceType == wi::input::Controller::XINPUT)
			{
				wi::input::xinput::SetControllerFeedback(data, controller.deviceIndex);
			}
			else if (controller.deviceType == wi::input::Controller::RAWINPUT)
			{
				wi::rawgamepad::SetControllerFeedback(data, controller.deviceIndex);
			}
		}
	}

	wi::vector<wi::input::Controller>& GetGamepadState()
	{
		return wi::input::controllers;
	}

	wi::vector<uint32_t>& GetGamepadPressed()
	{
		return wi::gamepad::pressed;
	}

	wi::vector<uint32_t>& GetGamepadReleased()
	{
		return wi::gamepad::released;
	}
}
