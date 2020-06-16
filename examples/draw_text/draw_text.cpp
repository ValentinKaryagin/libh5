#include "..\..\libh5\h5.h"

int main()
{
	// result
	int32_t error_level = 0;

	// window
	h5::application_window window;
	error_level = window.init(128, 128, 1280, 720, u("h5 draw text example"), nullptr, true, true);

	// render
	h5::render_device render;
	if (!error_level)
	{
		error_level = render.init(&window, false, true);
	}	

	// sprite
	h5::sprite_font font;
	if (!error_level)
	{
		error_level = render.create_sprite_font(u("consolas14.spritefont"), &font);
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

			// draw text
			render.draw_text(&font, u("hello, text!"), h5::vector2(10.0f, 10.0f), h5::colorf_rgba::red);
			render.draw_text(&font, u("hello, another one text!"), h5::vector2(10.0f, 30.0f), h5::colorf_rgba::green);
			render.draw_text(&font, u("hello, and another one text!\nwith translation"), h5::vector2(10.0f, 50.0f), h5::colorf_rgba::blue);

			// rendering
			error_level = render.present();			
		}		
	}	

	return error_level;
}
