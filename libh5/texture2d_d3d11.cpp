#include "texture2d_d3d11.h"

#include "d3d11_private.h"

namespace h5
{
	texture2d_d3d11::texture2d_d3d11() {}

	texture2d_d3d11::~texture2d_d3d11()
	{
		terminate();
	}

	int32_t texture2d_d3d11::load(const std::wstring &filename)
	{
		image = new (std::nothrow) DirectX::ScratchImage();
		if (!image)
		{
			return ERRLVL_DX_MEMORY_ALLOCATE;
		}

		wchar_t file_ext[_MAX_EXT]{};
		_wsplitpath_s(filename.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, file_ext, _MAX_EXT);		

		if (_wcsicmp(file_ext, u(".dds")) == 0)
		{
			if (FAILED(DirectX::LoadFromDDSFile(filename.c_str(), 0, nullptr, *image)))
			{
				return ERRLVL_DX_LOAD_FROM_DDS_FILE;
			}
		}
		else if (_wcsicmp(file_ext, u(".tga")) == 0)
		{
			if (FAILED(DirectX::LoadFromTGAFile(filename.c_str(), nullptr, *image)))
			{
				return ERRLVL_DX_LOAD_FROM_TGA_FILE;
			}
		}
		else if (_wcsicmp(file_ext, u(".hdr")) == 0)
		{
			if (FAILED(DirectX::LoadFromHDRFile(filename.c_str(), nullptr, *image)))
			{
				return ERRLVL_DX_LOAD_FROM_HDR_FILE;
			}
		}
		else
		{
			if (FAILED(DirectX::LoadFromWICFile(filename.c_str(), 0, nullptr, *image)))
			{
				return ERRLVL_DX_LOAD_FROM_WIC_FILE;
			}
		}	

		return 0;
	}

	void texture2d_d3d11::terminate()
	{
		if (image)
		{
			image->Release();
			delete image;
		}		
	}
}
