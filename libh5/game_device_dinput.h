#pragma once

#include "input_device_dinput.h"

#define DINPUT_DIJOYSTATE_SIZE 80

namespace h5
{
	class application_window_windows;

	class game_device_dinput
		: public input_device_dinput
	{
	public:
		enum class axis
			: size_t
		{
			x,
			y,
			z,
			rx,
			ry,
			rz,
			max
		};

		enum class slider
			: size_t
		{
			slider0,
			slider1,
			max
		};

		enum class button
			: size_t
		{
			mb0,
			mb1,
			mb2,
			mb3,
			mb4,
			mb5,
			mb6,
			mb7,
			mb8,
			mb9,
			mb10,
			mb11,
			mb12,
			mb13,
			mb14,
			mb15,
			mb16,
			mb17,
			mb18,
			mb19,
			mb20,
			mb21,
			mb22,
			mb23,
			mb24,
			mb25,
			mb26,
			mb27,
			mb28,
			mb29,
			mb30,
			mb31,
			max
		};

		enum class pov
			: size_t
		{
			pov0,
			pov1,
			pov2,
			pov3,
			max
		};

	protected:
		char state[DINPUT_DIJOYSTATE_SIZE]{};
		event events[(size_t)button::max] = {};

	public:
		game_device_dinput();
		virtual ~game_device_dinput();

	public:
		int32_t init(const application_window_windows *window);
		int32_t init(const application_window_windows *window, const guid &id, const std::wstring &name);

	public:
		int32_t read() override;
		void clear_state() override;

	public:
		bool is_event(button button, event event);
		int32_t get_axis(axis axis);
		int32_t get_slider(slider slider);
		int32_t get_pov(pov pov);
	};
}
