#pragma once

#include "d3d11_public.h"

namespace h5
{
	class render_buffer_d3d11
	{
	public:
		enum class type
		{
			vertex,
			index,
			constant,
			structured_buffer
		};

	public:
		friend class render_device_d3d11;

	protected:
		ID3D11Buffer *buffer = nullptr;

	public:
		render_buffer_d3d11();
		virtual ~render_buffer_d3d11();

	public:
		void terminate();

	public:
		bool is_valid();
	};
}
