#pragma once

#include "dinput_public.h"
#include "platform.h"

struct IDirectInput8W;
struct IDirectInputDevice8W;

namespace h5
{
	class input_device_dinput
	{
	public:
		enum class type
		{
			unknown,
			mouse,
			keyboard,
			game
		};	

		enum class event
		{
			none,
			pressed,
			repeated,
			released
		};

	public:
		typedef int32_t(enum_devices_func)(guid *, input_device_dinput::type, const wchar_t *);

	public:
		static int64_t refs;

	protected:
		guid device_id;
		std::wstring device_name;

	public:
		static void inc_ref();
		static void dec_ref();

	public:
		static IDirectInput8W *di;

	protected:		
		unsigned long long		wnd		= 0;
		IDirectInputDevice8W	*device	= nullptr;

	public:
		void terminate();

	public:
		virtual int32_t read() = 0;
		virtual void clear_state() = 0;

	public:
		const guid *get_device_id() const;
		const std::wstring get_device_name() const;

	public:
		static int32_t enum_devices(enum_devices_func *func_ptr);
	};
}
