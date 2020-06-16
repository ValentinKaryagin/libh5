#include "input_device_dinput.h"

#include "dinput_private.h"

namespace h5
{
	int64_t input_device_dinput::refs		= 0;
	IDirectInput8W *input_device_dinput::di	= nullptr;

	BOOL __stdcall enum_devices_callback(LPCDIDEVICEINSTANCEW lpddi, LPVOID pvRef);

	void input_device_dinput::inc_ref()
	{
		if (InterlockedIncrement64(&input_device_dinput::refs) == 1)
		{
			assert(DirectInput8Create(
				GetModuleHandle(nullptr),
				DIRECTINPUT_VERSION,
				IID_IDirectInput8W,
				reinterpret_cast<void **>(&input_device_dinput::di),
				nullptr) == 0);
		}
	}

	void input_device_dinput::dec_ref()
	{
		if (!InterlockedDecrement64(&input_device_dinput::refs))
		{
			input_device_dinput::di->Release();
			input_device_dinput::di = nullptr;
		}
	}

	void input_device_dinput::terminate()
	{
		clear_state();

		DINPUT_SAFE_RELEASE(device);
		wnd = 0;

		input_device_dinput::dec_ref();
	}
	
	const guid *input_device_dinput::get_device_id() const
	{
		return &device_id;
	}

	const std::wstring input_device_dinput::get_device_name() const
	{
		return device_name;
	}

	int32_t input_device_dinput::enum_devices(enum_devices_func *func_ptr)
	{
		int32_t error_level = 0;

		input_device_dinput::inc_ref();

		if (FAILED(di->EnumDevices(DI8DEVTYPE_MOUSE, &enum_devices_callback, func_ptr, DIEDFL_ATTACHEDONLY)))
		{
			error_level = ERRLVL_DINPUT_ENUM_KEYBOARD_DEVICES;
		}

		if (FAILED(di->EnumDevices(DI8DEVTYPE_KEYBOARD, &enum_devices_callback, func_ptr, DIEDFL_ATTACHEDONLY)))
		{
			error_level = ERRLVL_DINPUT_ENUM_KEYBOARD_DEVICES;
		}

		if (FAILED(di->EnumDevices(DI8DEVTYPE_JOYSTICK, &enum_devices_callback, func_ptr, DIEDFL_ATTACHEDONLY)))
		{
			error_level = ERRLVL_DINPUT_ENUM_KEYBOARD_DEVICES;
		}

		if (FAILED(di->EnumDevices(DI8DEVTYPE_GAMEPAD, &enum_devices_callback, func_ptr, DIEDFL_ATTACHEDONLY)))
		{
			error_level = ERRLVL_DINPUT_ENUM_KEYBOARD_DEVICES;
		}

		input_device_dinput::dec_ref();

		return error_level;
	}

	BOOL __stdcall enum_devices_callback(LPCDIDEVICEINSTANCEW lpddi, LPVOID pvRef)
	{
		BYTE type		= GET_DIDEVICE_TYPE(lpddi->dwDevType);
		BYTE sub_type	= GET_DIDEVICE_SUBTYPE(lpddi->dwDevType);

		guid id((const char *)&lpddi->guidInstance);

		if (type == DI8DEVTYPE_MOUSE)
		{
			if (pvRef)
			{
				int32_t error_level = ((input_device_dinput::enum_devices_func *)pvRef)(&id, input_device_dinput::type::mouse, lpddi->tszInstanceName);
			}			
		}
		else if (type == DI8DEVTYPE_KEYBOARD)
		{
			if (pvRef)
			{
				int32_t error_level = ((input_device_dinput::enum_devices_func *)pvRef)(&id, input_device_dinput::type::keyboard, lpddi->tszInstanceName);
			}
		}
		else if (type == DI8DEVTYPE_JOYSTICK || type == DI8DEVTYPE_GAMEPAD)
		{
			if (pvRef)
			{
				int32_t error_level = ((input_device_dinput::enum_devices_func *)pvRef)(&id, input_device_dinput::type::game, lpddi->tszInstanceName);
			}			
		}
		else
		{
			if (pvRef)
			{
				int32_t error_level = ((input_device_dinput::enum_devices_func *)pvRef)(&id, input_device_dinput::type::unknown, lpddi->tszInstanceName);
			}			
		}

		return DIENUM_CONTINUE;
	}
}
