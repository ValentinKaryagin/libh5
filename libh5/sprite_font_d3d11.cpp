#include "sprite_font_d3d11.h"

#include "render_device_d3d11.h"

#include "d3d11_private.h"

namespace h5
{
	sprite_font_d3d11::sprite_font_d3d11() {}

	sprite_font_d3d11::~sprite_font_d3d11()
	{
		terminate();
	}

	void sprite_font_d3d11::terminate()
	{
		if (font)
		{
			delete font;
			font = nullptr;
		}
	}
}
