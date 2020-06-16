#pragma once

#include "d3d11_public.h"

namespace h5
{
	class shader_res_d3d11
	{
	public:
		friend class render_device_d3d11;

	protected:
		ID3D11ShaderResourceView *shader_res = nullptr;

	public:
		shader_res_d3d11();
		virtual ~shader_res_d3d11();

	public:
		void terminate();
	};
}
