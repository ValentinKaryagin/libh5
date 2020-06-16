#include "shader_res_d3d11.h"

#include "d3d11_private.h"

namespace h5
{
	shader_res_d3d11::shader_res_d3d11() {}

	shader_res_d3d11::~shader_res_d3d11()
	{
		terminate();
	}

	void shader_res_d3d11::terminate()
	{
		D3D11_SAFE_RELEASE(shader_res);
	}
}
