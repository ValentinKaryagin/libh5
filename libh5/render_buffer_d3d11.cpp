#include "render_buffer_d3d11.h"

#include "d3d11_private.h"

namespace h5
{
	render_buffer_d3d11::render_buffer_d3d11() {}

	render_buffer_d3d11::~render_buffer_d3d11()
	{
		terminate();
	}

	void render_buffer_d3d11::terminate()
	{
		D3D11_SAFE_RELEASE(buffer);
	}

	bool render_buffer_d3d11::is_valid()
	{
		return buffer;
	}
}
