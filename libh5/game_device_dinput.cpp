#include "game_device_dinput.h"

#include "application_window_windows.h"

#include "dinput_private.h"

namespace h5
{
	game_device_dinput::game_device_dinput() {}

	game_device_dinput::~game_device_dinput()
	{
		terminate();
	}

	int32_t game_device_dinput::init(const application_window_windows *window)
	{
		if (!window)
		{
			return ERRLVL_DINPUT_NULL_WINDOW;
		}

		input_device_dinput::inc_ref();

		wnd = window->get_win32_handler();

		if (FAILED(di->CreateDevice(GUID_Joystick, &device, nullptr)))
		{
			return ERRLVL_DINPUT_CREATE_DEVICE;
		}

		if (FAILED(device->SetDataFormat(&c_dfDIJoystick)))
		{
			return ERRLVL_DINPUT_DEVICE_SET_DATA_FORMAT;
		}

		if (FAILED(device->SetCooperativeLevel((HWND)wnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		{
			return ERRLVL_DINPUT_DEVICE_SET_COOP_LEVEL;
		}

		if (FAILED(device->Acquire()))
		{
			return ERRLVL_DINPUT_DEVICE_ACQUIRE;
		}

		return 0;
	}

	int32_t game_device_dinput::init(const application_window_windows *window, const guid &id, const std::wstring &name)
	{
		if (!window)
		{
			return ERRLVL_DINPUT_NULL_WINDOW;
		}

		input_device_dinput::inc_ref();

		wnd = window->get_win32_handler();

		if (FAILED(di->CreateDevice(*id.get_win32_guid(), &device, nullptr)))
		{
			return ERRLVL_DINPUT_CREATE_DEVICE;
		}

		if (FAILED(device->SetDataFormat(&c_dfDIJoystick)))
		{
			return ERRLVL_DINPUT_DEVICE_SET_DATA_FORMAT;
		}

		if (FAILED(device->SetCooperativeLevel((HWND)wnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		{
			return ERRLVL_DINPUT_DEVICE_SET_COOP_LEVEL;
		}

		if (FAILED(device->Acquire()))
		{
			return ERRLVL_DINPUT_DEVICE_ACQUIRE;
		}

		device_id = id;
		device_name = name;

		return 0;
	}

	int32_t game_device_dinput::read()
	{
		HRESULT hres = device->GetDeviceState(sizeof(state), reinterpret_cast<void *>(&state));
		if (FAILED(hres))
		{
			if (hres == DIERR_INPUTLOST)
			{
				return ERRLVL_DINPUT_DEVICE_DISCONNECTED;
			}
			else if (hres == DIERR_NOTACQUIRED)
			{
				if (FAILED(device->Acquire()))
				{
					return ERRLVL_DINPUT_DEVICE_ACQUIRE;
				}
			}
		}
		else
		{
			for (size_t i = 0; i < (size_t)button::max; ++i)
			{
				if (((DIJOYSTATE *)state)->rgbButtons[i] & 0x80)
				{
					if (events[i] == event::none)
					{
						events[i] = event::pressed;

						DEBUG_WPRINTF("[%s:%s] -> [pressed:%zd]", device_id.to_wstring().c_str(), device_name.c_str(), i);
					}
					else if (events[i] == event::pressed)
					{
						events[i] = event::repeated;

						DEBUG_WPRINTF("[%s:%s] -> [repeated:%zd]", device_id.to_wstring().c_str(), device_name.c_str(), i);
					}
					else if (events[i] == event::repeated)
					{
						events[i] = event::repeated;
					}
					else
					{
						events[i] = event::pressed;

						DEBUG_WPRINTF("[%s:%s] -> [pressed:%zd]", device_id.to_wstring().c_str(), device_name.c_str(), i);
					}
				}
				else
				{
					if (events[i] == event::none)
					{
						events[i] = event::none;
					}
					else if (events[i] == event::pressed)
					{
						events[i] = event::released;

						DEBUG_WPRINTF("[%s:%s] -> [released:%zd]", device_id.to_wstring().c_str(), device_name.c_str(), i);
					}
					else if (events[i] == event::repeated)
					{
						events[i] = event::released;

						DEBUG_WPRINTF("[%s:%s] -> [released:%zd]", device_id.to_wstring().c_str(), device_name.c_str(), i);
					}
					else
					{
						events[i] = event::none;
					}
				}
			}

#ifdef _DEBUG
			/*if (get_axis(axis::x))
			{
				DEBUG_WPRINTF("[%s:%s] -> [axis:x:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_axis(axis::x));
			}
			
			if (get_axis(axis::y))
			{
				DEBUG_WPRINTF("[%s:%s] -> [axis:y:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_axis(axis::y));
			}

			if (get_axis(axis::z))
			{
				DEBUG_WPRINTF("[%s:%s] -> [axis:z:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_axis(axis::z));
			}*/

			/*if (get_axis(axis::rx))
			{
				DEBUG_WPRINTF("[%s:%s] -> [axis:rx:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_axis(axis::rx));
			}

			if (get_axis(axis::ry))
			{
				DEBUG_WPRINTF("[%s:%s] -> [axis:ry:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_axis(axis::ry));
			}

			if (get_axis(axis::rz))
			{
				DEBUG_WPRINTF("[%s:%s] -> [axis:rz:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_axis(axis::rz));
			}*/

			/*if (get_slider(slider::slider0))
			{
				DEBUG_WPRINTF("[%s:%s] -> [slider:slider0:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_slider(slider::slider0));
			}

			if (get_slider(slider::slider1))
			{
				DEBUG_WPRINTF("[%s:%s] -> [slider:slider1:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_slider(slider::slider1));
			}

			if (get_pov(pov::pov0) != -1)
			{
				DEBUG_WPRINTF("[%s:%s] -> [pov:pov0:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_pov(pov::pov0));
			}

			if (get_pov(pov::pov1) != -1)
			{
				DEBUG_WPRINTF("[%s:%s] -> [pov:pov1:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_pov(pov::pov1));
			}

			if (get_pov(pov::pov2) != -1)
			{
				DEBUG_WPRINTF("[%s:%s] -> [pov:pov2:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_pov(pov::pov2));
			}

			if (get_pov(pov::pov3) != -1)
			{
				DEBUG_WPRINTF("[%s:%s] -> [pov:pov3:%d]", device_id.to_wstring().c_str(), device_name.c_str(), get_pov(pov::pov3));
			}*/
#endif
		}

		return 0;
	}

	void game_device_dinput::clear_state()
	{
		memset(state, 0, sizeof(state));
		memset(events, 0, sizeof(events));
	}

	bool game_device_dinput::is_event(button button, event event)
	{
		return events[static_cast<size_t>(button)] == event;
	}

	int32_t game_device_dinput::get_axis(axis axis)
	{
		return
			axis == axis::x ? ((DIJOYSTATE *)state)->lX :
			axis == axis::y ? ((DIJOYSTATE *)state)->lY :
			axis == axis::z ? ((DIJOYSTATE *)state)->lZ :
			axis == axis::rx ? ((DIJOYSTATE *)state)->lRx :
			axis == axis::ry ? ((DIJOYSTATE *)state)->lRy :
			axis == axis::rz ? ((DIJOYSTATE *)state)->lRz :
			0;
	}

	int32_t game_device_dinput::get_slider(slider slider)
	{
		return
			slider == slider::slider0 ? ((DIJOYSTATE *)state)->rglSlider[0] :
			slider == slider::slider1 ? ((DIJOYSTATE *)state)->rglSlider[1] :
			0;
	}

	int32_t game_device_dinput::get_pov(pov pov)
	{
		return
			pov == pov::pov0 ? ((DIJOYSTATE *)state)->rgdwPOV[0] :
			pov == pov::pov1 ? ((DIJOYSTATE *)state)->rgdwPOV[1] :
			pov == pov::pov2 ? ((DIJOYSTATE *)state)->rgdwPOV[2] :
			pov == pov::pov3 ? ((DIJOYSTATE *)state)->rgdwPOV[3] :
			0;
	}
}
