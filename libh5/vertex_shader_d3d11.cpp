#include "vertex_shader_d3d11.h"

#include "d3d11_private.h"

#include <iostream>

namespace h5
{
	vertex_shader_d3d11::vertex_shader_d3d11() {}

	vertex_shader_d3d11::~vertex_shader_d3d11()
	{
		terminate();
	}

	int32_t vertex_shader_d3d11::load(const std::wstring &filename, const std::string &entry_point)
	{
		ID3DBlob *errors = nullptr;
		if (FAILED(D3DCompileFromFile(
			filename.c_str(),
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entry_point.c_str(),
			"vs_5_0",
			D3DCOMPILE_ENABLE_STRICTNESS,
			0,
			&buffer,
			&errors)))
		{
			if (errors)
			{
#ifdef _DEBUG
				printf((char *)errors->GetBufferPointer());
#endif
				errors->Release();
			}

			return ERRLVL_D3D11_COMPILE_SHADER_FROM_FILE;
		}

		return 0;
	}

	void vertex_shader_d3d11::terminate()
	{
		D3D11_SAFE_RELEASE(buffer);
		D3D11_SAFE_RELEASE(shader);
	}
}
