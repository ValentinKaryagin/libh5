#pragma once

#include "d3d11_public.h"

#ifndef LIBH5
#ifdef _DEBUG
#pragma comment(lib, "..\\..\\third_party\\DirectXTex-aug2019\\DirectXTex\\Bin\\Desktop_2019_Win10\\x64\\Debug\\DirectXTex.lib")
#else
#pragma comment(lib, "..\\..\\third_party\\DirectXTex-aug2019\\DirectXTex\\Bin\\Desktop_2019_Win10\\x64\\Release\\DirectXTex.lib")
#endif
#endif

namespace h5
{
	class texture2d_d3d11
	{
	public:
		friend class render_device_d3d11;

	protected:
		DirectX::ScratchImage *image = nullptr;

	public:
		texture2d_d3d11();
		virtual ~texture2d_d3d11();

	public:
		int32_t load(const std::wstring &filename);
		void terminate();
	};
}
