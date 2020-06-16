#include "..\..\libh5\h5.h"

#include <iostream>

int main()
{
	int32_t error_level = 0;

	h5::application_window window;
	error_level = window.init(128, 128, 1280, 720, u("h5 input example"), nullptr, true, true);

	h5::keyboard_device keyboard;
	if (!error_level)
	{
		error_level = keyboard.init(&window);
	}

	h5::mouse_device mouse;
	if (!error_level)
	{
		error_level = mouse.init(&window);
	}
	
	while (!window.is_exit_requested() && !error_level)
	{
		error_level = window.proc();
		
		if (!error_level)
		{
			error_level = keyboard.read();
		}
		
		if (!error_level)
		{
			error_level = mouse.read();
		}

		if (!error_level)
		{
			if (keyboard.is_event(h5::keyboard_device::key::w, h5::input_device::event::pressed))
			{
				printf("w key pressed\n");
			}

			if (keyboard.is_event(h5::keyboard_device::key::w, h5::input_device::event::released))
			{
				printf("w key released\n");
			}

			if (mouse.is_event(h5::mouse_device::button::mb0, h5::input_device::event::pressed))
			{
				printf("left mouse button pressed\n");
			}

			if (mouse.is_event(h5::mouse_device::button::mb0, h5::input_device::event::released))
			{
				printf("left mouse button released\n");
			}
		}		
	}

	return error_level;
}
