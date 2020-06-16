#pragma once

#include "d3d11_public.h"

#include "math.h"
#include "color.h"

#include "render_buffer_d3d11.h"

namespace h5
{
	class application_window_windows;
	class render_buffer_d3d11;
	class texture2d_d3d11;
	class vertex_shader_d3d11;
	class pixel_shader_d3d11;
	class shader_res_d3d11;
	class sprite_font_d3d11;

	struct vertex;

	class render_device_d3d11
	{
	public:
		enum class state
		{
			solid,
			wireframe
		};

	protected:
		ID3D11Device			*device					= nullptr;
		ID3D11DeviceContext		*device_context			= nullptr;
		IDXGISwapChain			*swap_chain				= nullptr;
		ID3D11RenderTargetView	*rtv					= nullptr;
		ID3D11DepthStencilView	*dsv					= nullptr;
		ID3D11Texture2D			*ds_buffer				= nullptr;
		ID3D11InputLayout		*layout					= nullptr;		
		ID3D11RasterizerState	*solid_rs				= nullptr;
		ID3D11RasterizerState	*wireframe_rs			= nullptr;
		ID3D11SamplerState		*sampler_state			= nullptr;

	protected:
		DirectX::SpriteBatch	*sprite_batch			= nullptr;

#ifdef _DEBUG
	protected:
		size_t objects_to_draw_count	= 0;
		size_t draw_calls_count			= 0;
#endif

	protected:
		bool is_vsync = false;

	public:
		render_device_d3d11();
		virtual ~render_device_d3d11();

	public:
		int32_t init(application_window_windows *window, bool is_fullscreen = false, bool is_vsync_enabled = true);
		void terminate();
		void enable_vsync(bool is_enabled = true);

	public:
		void clear(const colorf_rgba &color = colorf_rgba::black);
		int32_t present() const;

	public:
		void draw(size_t vertices_count);
		void draw_indexed(size_t indices_count);
		void draw_instanced(size_t vertices_count_per_instance, size_t instances_count);
		void draw_indexed_instanced(size_t indices_count_per_instance, size_t instances_count);

	public:
		void draw_text(const sprite_font_d3d11 *font, const std::wstring &text, const vector2 &coord, const colorf_rgba &color);
		void draw_text(const sprite_font_d3d11 *font, const std::string &text, const vector2 &coord, const colorf_rgba &color);

#ifdef _DEBUG
	public:
		__forceinline const size_t & get_objects_to_draw_count() const
		{
			return objects_to_draw_count;
		}

		__forceinline const size_t & get_draw_calls_count() const
		{
			return draw_calls_count;
		}
#endif

	public:
		int32_t create_buffer(render_buffer_d3d11::type type, void *data, size_t elements_count, size_t element_size, render_buffer_d3d11 *out) const;
		int32_t create_shader_res(render_buffer_d3d11 *buffer, size_t elements_count, shader_res_d3d11 *out) const;
		int32_t create_shader_res(texture2d_d3d11 *textures, shader_res_d3d11 *out) const;
		int32_t create_vertex_shader(vertex_shader_d3d11 *out);
		int32_t create_pixel_shader(pixel_shader_d3d11 *out);
		int32_t create_sprite_font(const std::wstring &filename, sprite_font_d3d11 *out);

	public:
		void set_vertex_buffer(render_buffer_d3d11 *buffer);
		void set_index_buffer(render_buffer_d3d11 *buffer);
		void set_vertex_shader(vertex_shader_d3d11 *shader);
		void set_pixel_shader(pixel_shader_d3d11 *shader);

	public:		
		void update_constant_buffer(render_buffer_d3d11 *buffer, void *data, size_t index) const;

	public:
		void set_vertex_shader_constant_buffer(render_buffer_d3d11 *buffer, size_t index);
		void set_pixel_shader_constant_buffer(render_buffer_d3d11 *buffer, size_t index);
		void set_vertex_shader_res(shader_res_d3d11 *res, size_t index);
		void set_pixel_shader_res(shader_res_d3d11* res, size_t index);

	public:
		void set_render_state(state rs);

	public:
		int32_t resize_viewport(uint32_t x, uint32_t y);
	};
}
