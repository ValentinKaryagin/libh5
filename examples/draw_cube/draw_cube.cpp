#include "..\..\libh5\h5.h"

#include <vector>
#include <iostream>

// cube vertices
std::vector<h5::vertex> vertices =
{
	h5::vertex(h5::vector3(-1.0f, -1.0f, -1.0f)),
	h5::vertex(h5::vector3(-1.0f, +1.0f, -1.0f)),
	h5::vertex(h5::vector3(+1.0f, +1.0f, -1.0f)),
	h5::vertex(h5::vector3(+1.0f, -1.0f, -1.0f)),
	h5::vertex(h5::vector3(-1.0f, -1.0f, +1.0f)),
	h5::vertex(h5::vector3(-1.0f, +1.0f, +1.0f)),
	h5::vertex(h5::vector3(+1.0f, +1.0f, +1.0f)),
	h5::vertex(h5::vector3(+1.0f, -1.0f, +1.0f))
};

// cube indices
std::vector<uint32_t> indices = { 0, 1, 2, 0, 2, 3, 4, 6, 5, 4, 7, 6, 4, 5, 1, 4, 1, 0, 3, 2, 6, 3, 6, 7, 1, 5, 6, 1, 6, 2, 4, 0, 3, 4, 3, 7 };

int main()
{
	// result
	int32_t error_level = 0;
	
	// window
	h5::application_window window;	
	error_level = window.init(128, 128, 1280, 720, u("h5 draw cube example"), nullptr, true, true);
	
	// render
	h5::render_device render;
	if (!error_level)
	{
		error_level = render.init(&window, false, true);
	}

	// vertices buffer
	h5::buffer vb;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::vertex, vertices.data(), vertices.size(), sizeof(h5::vertex), &vb);
		if (!error_level)
		{
			render.set_vertex_buffer(&vb);
		}
	}	

	// indices buffer
	h5::buffer ib;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::index, indices.data(), indices.size(), sizeof(uint32_t), &ib);
		if (!error_level)
		{
			render.set_index_buffer(&ib);
		}
	}	

	// vertex shader
	h5::vertex_shader vs;
	if (!error_level)
	{
		error_level = vs.load(u("draw_cube.hlsl"), "vs");
		if (!error_level)
		{
			error_level = render.create_vertex_shader(&vs);
			if (!error_level)
			{
				render.set_vertex_shader(&vs);
			}
		}
	}

	// pixel shader
	h5::pixel_shader ps;
	if (!error_level)
	{
		error_level = ps.load(u("draw_cube.hlsl"), "ps");
		if (!error_level)
		{
			error_level = render.create_pixel_shader(&ps);
			if (!error_level)
			{
				render.set_pixel_shader(&ps);
			}			
		}
	}

	// matrices
	h5::matrix w, v, p, wvp;
	w.translation(h5::vector3::zero);
	v.look_at(h5::vector3(-10.0f, 0.0f, 0.0f), h5::vector3::zero, h5::vector3::up);
	p.perspective(deg_to_rad(60.0f), 1280.0f / 720.0f, 1.0f, 10240.0f);
	wvp = w * v * p;

	// constant buffer
	h5::buffer cb;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::constant, &wvp, 1, sizeof(h5::matrix), &cb);
		if (!error_level)
		{
			render.set_vertex_shader_constant_buffer(&cb, 0);
		}
	}
	
	// loop
	while (!window.is_exit_requested() && !error_level)
	{
		// window events processing
		error_level = window.proc();
			
		if (!error_level)
		{
			// clear viewport
			render.clear(h5::colorf_rgba::black);

			// draw cube
			render.draw_indexed(indices.size());

			// rendering
			error_level = render.present();
		}			
	}

	return error_level;
}
