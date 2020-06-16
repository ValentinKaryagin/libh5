#pragma once

#include "render_buffer_d3d11.h"
#include "render_device_d3d11.h"
#include "vertex_shader_d3d11.h"
#include "pixel_shader_d3d11.h"
#include "texture2d_d3d11.h"
#include "shader_res_d3d11.h"
#include "sprite_font_d3d11.h"

#ifndef LIBH5
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "d3dcompiler.lib")
#endif

namespace h5
{
	typedef render_buffer_d3d11		render_buffer;
	typedef render_device_d3d11		render_device;
	typedef vertex_shader_d3d11		vertex_shader;
	typedef pixel_shader_d3d11		pixel_shader;
	typedef texture2d_d3d11			texture2d;
	typedef shader_res_d3d11		shader_res;
	typedef sprite_font_d3d11		sprite_font;
}
