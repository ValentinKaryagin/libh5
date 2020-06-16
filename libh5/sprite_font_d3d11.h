#pragma once

#include "d3d11_public.h"

#ifndef LIBH5
#ifdef _DEBUG
#pragma comment(lib, "..\\..\\third_party\\DirectXTK-aug2019\\Bin\\Desktop_2019_Win10\\x64\\Debug\\DirectXTK.lib")
#else
#pragma comment(lib, "..\\..\\third_party\\DirectXTK-aug2019\\Bin\\Desktop_2019_Win10\\x64\\Release\\DirectXTK.lib")
#endif
#endif

namespace h5
{
	class sprite_font_d3d11
	{
	public:
		friend class render_device_d3d11;

	protected:
		DirectX::SpriteFont *font = nullptr;

	public:
		sprite_font_d3d11();
		virtual ~sprite_font_d3d11();

	public:
		void terminate();
	};
}
