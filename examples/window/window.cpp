#include "..\..\libh5\h5.h"

#include <iostream>

int main()
{
	// result
	int32_t error_level = 0;

	// window
	h5::application_window window;
	error_level = window.init(128, 128, 1280, 720, u("h5 window example"), nullptr, true, true);
	
	while (!window.is_exit_requested() && !error_level)
	{
		// window events processing
		error_level = window.proc();			
	}		

	return error_level;
}
