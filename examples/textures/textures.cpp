#include "..\..\libh5\h5.h"

#include <ctime>
#include <string>
#include <vector>

constexpr size_t OBJECTS_COUNT = 1000;

std::vector<h5::vertex> vertices =
{
	h5::vertex(h5::vector3(-1.0f, -1.0f, -1.0f), h5::vector2(0.0f, 1.0f)),
	h5::vertex(h5::vector3(-1.0f, +1.0f, -1.0f), h5::vector2(0.0f, 0.0f)),
	h5::vertex(h5::vector3(+1.0f, +1.0f, -1.0f), h5::vector2(1.0f, 0.0f)),
	h5::vertex(h5::vector3(+1.0f, -1.0f, -1.0f), h5::vector2(1.0f, 1.0f)),
	h5::vertex(h5::vector3(-1.0f, -1.0f, +1.0f), h5::vector2(1.0f, 1.0f)),
	h5::vertex(h5::vector3(+1.0f, -1.0f, +1.0f), h5::vector2(0.0f, 1.0f)),
	h5::vertex(h5::vector3(+1.0f, +1.0f, +1.0f), h5::vector2(0.0f, 0.0f)),
	h5::vertex(h5::vector3(-1.0f, +1.0f, +1.0f), h5::vector2(1.0f, 0.0f)),
	h5::vertex(h5::vector3(-1.0f, +1.0f, -1.0f), h5::vector2(0.0f, 1.0f)),
	h5::vertex(h5::vector3(-1.0f, +1.0f, +1.0f), h5::vector2(0.0f, 0.0f)),
	h5::vertex(h5::vector3(+1.0f, +1.0f, +1.0f), h5::vector2(1.0f, 0.0f)),
	h5::vertex(h5::vector3(+1.0f, +1.0f, -1.0f), h5::vector2(1.0f, 1.0f)),
	h5::vertex(h5::vector3(-1.0f, -1.0f, -1.0f), h5::vector2(1.0f, 1.0f)),
	h5::vertex(h5::vector3(+1.0f, -1.0f, -1.0f), h5::vector2(0.0f, 1.0f)),
	h5::vertex(h5::vector3(+1.0f, -1.0f, +1.0f), h5::vector2(0.0f, 0.0f)),
	h5::vertex(h5::vector3(-1.0f, -1.0f, +1.0f), h5::vector2(1.0f, 0.0f)),
	h5::vertex(h5::vector3(-1.0f, -1.0f, +1.0f), h5::vector2(0.0f, 1.0f)),
	h5::vertex(h5::vector3(-1.0f, +1.0f, +1.0f), h5::vector2(0.0f, 0.0f)),
	h5::vertex(h5::vector3(-1.0f, +1.0f, -1.0f), h5::vector2(1.0f, 0.0f)),
	h5::vertex(h5::vector3(-1.0f, -1.0f, -1.0f), h5::vector2(1.0f, 1.0f)),
	h5::vertex(h5::vector3(+1.0f, -1.0f, -1.0f), h5::vector2(0.0f, 1.0f)),
	h5::vertex(h5::vector3(+1.0f, +1.0f, -1.0f), h5::vector2(0.0f, 0.0f)),
	h5::vertex(h5::vector3(+1.0f, +1.0f, +1.0f), h5::vector2(1.0f, 0.0f)),
	h5::vertex(h5::vector3(+1.0f, -1.0f, +1.0f), h5::vector2(1.0f, 1.0f))
};

std::vector<uint32_t> indices =
{ 0, 1, 2, 0, 2, 3,	4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };


int main()
{
	int32_t error_level = 0;

	std::srand(static_cast<uint32_t>(std::time(nullptr)));
		
	h5::application_window window;
	error_level = window.init(128, 128, 1280, 720, u("h5 textures example"));

	h5::render_device render;
	if (!error_level)
	{
		error_level = render.init(&window, false, true);
		if (!error_level)
		{
			render.set_render_state(h5::render_device::state::solid);
		}
	}	

	h5::buffer vb;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::vertex, vertices.data(), vertices.size(), sizeof(h5::vertex), &vb);
		if (!error_level)
		{
			render.set_vertex_buffer(&vb);
		}
	}

	h5::buffer ib;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::index, indices.data(), indices.size(), sizeof(uint32_t), &ib);
		if (!error_level)
		{
			render.set_index_buffer(&ib);
		}
	}

	h5::vertex_shader vs;
	if (!error_level)
	{
		error_level = vs.load(u("textures.hlsl"), "vs");
		if (!error_level)
		{
			error_level = render.create_vertex_shader(&vs);
			if (!error_level)
			{
				render.set_vertex_shader(&vs);
			}
		}
	}

	h5::pixel_shader ps;
	if (!error_level)
	{
		error_level = ps.load(u("textures.hlsl"), "ps");
		if (!error_level)
		{
			error_level = render.create_pixel_shader(&ps);
			if (!error_level)
			{
				render.set_pixel_shader(&ps);
			}
		}
	}

	h5::matrix v, p, vp, wvp;
	v.look_at(h5::vector3(-40.0f, 10.0f, 10.0f), h5::vector3::zero, h5::vector3::up);
	p.perspective(deg_to_rad(60.0f), 1280.0f / 720.0f, 1.0f, 10240.0f);
	vp = v * p;

	std::vector<h5::matrix> w_matrices;
	for (size_t i = 0; i < OBJECTS_COUNT; ++i)
	{
		h5::matrix w;
		w.translation(h5::vector3(
			static_cast<float>(std::rand() % 40 - 20),
			static_cast<float>(std::rand() % 40 - 20),
			static_cast<float>(std::rand() % 40 - 20)));
		w_matrices.push_back(w);
	}
	
	h5::buffer cb;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::constant, &vp, 1, sizeof(h5::matrix), &cb);
		if (!error_level)
		{
			render.set_vertex_shader_constant_buffer(&cb, 0);
		}
	}

	h5::buffer srb;
	h5::shader_res sr;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::structured_buffer, w_matrices.data(), OBJECTS_COUNT, sizeof(h5::matrix), &srb);
		if (!error_level)
		{
			error_level = render.create_shader_res(&srb, OBJECTS_COUNT, &sr);
			if (!error_level)
			{
				render.set_vertex_shader_res(&sr, 0);
			}
		}
	}

	h5::texture2d texture_color;
	h5::shader_res texture_sr;
	if (!error_level)
	{
		error_level = texture_color.load(u("textures.png"));
		if (!error_level)
		{
			error_level = render.create_shader_res(&texture_color, &texture_sr);
			if (!error_level)
			{
				render.set_pixel_shader_res(&texture_sr, 0);
			}
		}
	}

	while (!window.is_exit_requested() && !error_level)
	{
		error_level = window.proc();

		if (!error_level)
		{
			render.clear(h5::colorf_rgba::black);

			render.draw_indexed_instanced(indices.size(), OBJECTS_COUNT);

			error_level = render.present();
		}
	}

	return error_level;
}
