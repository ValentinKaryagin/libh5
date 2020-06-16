#pragma once

#include "d3d11_public.h"

namespace h5
{
	class pixel_shader_d3d11
	{
	public:
		friend class render_device_d3d11;

	protected:
		ID3D10Blob			*buffer	= nullptr;
		ID3D11PixelShader	*shader	= nullptr;

	public:
		pixel_shader_d3d11();
		virtual ~pixel_shader_d3d11();

	public:
		int32_t load(const std::wstring &filename, const std::string &entry_point);
		void terminate();
	};
}
