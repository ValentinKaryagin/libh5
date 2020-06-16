#pragma once

#include "input_device_dinput.h"

#define DINPUT_DIMOUSESTATE_SIZE 16

namespace h5
{
	class application_window_windows;

	class mouse_device_dinput
		: public input_device_dinput
	{
	public:
		enum class axis
			: size_t
		{
			x,
			y,
			z,
			max
		};

		enum class button
			: size_t
		{
			mb0 = 0,
			mb1 = 1,
			mb2 = 2,
			mb3 = 3,
			max
		};

	protected:
		char state[DINPUT_DIMOUSESTATE_SIZE]{};
		event events[(size_t)button::max] = {};

	public:
		mouse_device_dinput();
		virtual ~mouse_device_dinput();

	public:
		int32_t init(const application_window_windows *window);
		int32_t init(const application_window_windows *window, const guid &id, const std::wstring &name);

	public:
		int32_t read() override;
		void clear_state() override;

	public:
		bool is_event(button button, event event);
		int32_t get_axis(axis axis);
	};
}
