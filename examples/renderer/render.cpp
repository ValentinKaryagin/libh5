#include "..\..\libh5\h5.h"

#include <iostream>

int main()
{
	// result
	int32_t error_level = 0;

	// window
	h5::application_window window;	
	error_level = window.init(128, 128, 1280, 720, u("h5 render example"), nullptr, true, true);

	// render
	h5::render_device render;
	if (!error_level)
	{
		error_level = render.init(&window, false, true);
	}	

	// loop
	while (!window.is_exit_requested() && !error_level)
	{
		// window events processing
		error_level = window.proc();

		if (!error_level)
		{
			// clear viewport
			render.clear(h5::colorf_rgba::black);

			// rendering
			error_level = render.present();
		}		
	}

	return error_level;
}
