#include "mouse_device_dinput.h"

#include "application_window_windows.h"

#include "dinput_private.h"

namespace h5
{
	mouse_device_dinput::mouse_device_dinput() {}

	mouse_device_dinput::~mouse_device_dinput()
	{
		terminate();
	}

	int32_t mouse_device_dinput::init(const application_window_windows *window)
	{
		if (!window)
		{
			return ERRLVL_DINPUT_NULL_WINDOW;
		}

		input_device_dinput::inc_ref();

		wnd = window->get_win32_handler();		

		if (FAILED(di->CreateDevice(GUID_SysMouse, &device, nullptr)))
		{
			return ERRLVL_DINPUT_CREATE_DEVICE;
		}

		if (FAILED(device->SetDataFormat(&c_dfDIMouse)))
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

	int32_t mouse_device_dinput::init(const application_window_windows *window, const guid &id, const std::wstring &name)
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

		if (FAILED(device->SetDataFormat(&c_dfDIMouse)))
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

	int32_t mouse_device_dinput::read()
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
				if (((DIMOUSESTATE *)state)->rgbButtons[i] & 0x80)
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
		}

		return 0;
	}

	void mouse_device_dinput::clear_state()
	{
		memset(state, 0, sizeof(state));
		memset(events, 0, sizeof(events));
	}

	bool mouse_device_dinput::is_event(button button, event event)
	{
		return events[static_cast<size_t>(button)] == event;
	}

	int32_t mouse_device_dinput::get_axis(axis axis)
	{
		return
			axis == axis::x ? ((DIMOUSESTATE *)state)->lX :
			axis == axis::y ? ((DIMOUSESTATE *)state)->lY :
			axis == axis::z ? ((DIMOUSESTATE *)state)->lZ :
			0;
	}
}
