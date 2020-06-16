#include "..\..\libh5\h5.h"

#include <vector>
#include <iostream>
#include <ctime>

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
	error_level = window.init(128, 128, 1280, 720, u("h5 camera example"), nullptr, true, true);

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
	}

	// indices buffer
	h5::buffer ib;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::index, indices.data(), indices.size(), sizeof(uint32_t), &ib);
	}

	// vertex shader
	h5::vertex_shader vs;
	if (!error_level)
	{
		error_level = vs.load(u("camera.hlsl"), "vs");
		if (!error_level)
		{
			error_level = render.create_vertex_shader(&vs);
		}
	}

	// pixel shader
	h5::pixel_shader ps;
	if (!error_level)
	{
		error_level = ps.load(u("camera.hlsl"), "ps");
		if (!error_level)
		{
			error_level = render.create_pixel_shader(&ps);
		}
	}	

	// mouse
	h5::mouse_device mouse;
	if (!error_level)
	{
		error_level = mouse.init(&window);
	}

	// keyboard
	h5::keyboard_device keyboard;
	if (!error_level)
	{
		error_level = keyboard.init(&window);
	}

	// camera transform
	h5::transform_component camera_tr;
	camera_tr.set_translation(h5::vector3(-10.0f, 0.0f, 0.0f));

	// camera vp
	h5::matrix camera_v, camera_p, camera_vp;

	// cube setups
	h5::transform_component obj_tr;
	obj_tr.set_translation(h5::vector3(10.0f, 0.0f, 0.0f));

	// constant buffer
	h5::buffer cb;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::constant, (void *)&h5::matrix::identity, 1, sizeof(h5::matrix), &cb);
	}

	// sprite
	h5::sprite_font font;
	if (!error_level)
	{
		error_level = render.create_sprite_font(u("consolas16.spritefont"), &font);
	}
	
	time_t pre = clock(), post = 0;
	float delta_time = 0;
	const float speed = 3.0f;

	// loop
	while (!window.is_exit_requested() && !error_level)
	{
		// update timer
		post = clock();
		delta_time = static_cast<float>(post - pre) * 0.001f;
		pre = post;

		// window events processing
		error_level = window.proc();

		// read mouse			
		if (!error_level)
		{
			error_level = mouse.read();
		}

		// read keyboard			
		if (!error_level)
		{
			error_level = keyboard.read();
		}

		// process input
		if (!error_level)
		{
			// add z rotation
			camera_tr.add_rotation(
				h5::quat(
					camera_tr.get_up_direction(),
					deg_to_rad(static_cast<float>(mouse.get_axis(h5::mouse_device::axis::x)) * delta_time)
				)
			);

			// add y rotation
			camera_tr.add_rotation(
				h5::quat(
					camera_tr.get_right_direction(),
					deg_to_rad(static_cast<float>(mouse.get_axis(h5::mouse_device::axis::y)) * delta_time)
				)
			);

			// key pressed e
			if (keyboard.is_event(h5::keyboard_device::key::e, h5::input_device::event::repeated))
			{
				camera_tr.add_rotation(
					h5::quat(
						camera_tr.get_forward_direction(),
						deg_to_rad(-delta_time) * 90.0f
					)
				);
			}

			// key pressed q
			if (keyboard.is_event(h5::keyboard_device::key::q, h5::input_device::event::repeated))
			{
				camera_tr.add_rotation(
					h5::quat(
						camera_tr.get_forward_direction(),
						deg_to_rad(delta_time) * 90.0f
					)
				);
			}

			// key pressed w
			if (keyboard.is_event(h5::keyboard_device::key::w, h5::input_device::event::repeated))
			{
				// move camera actor matrix forward
				camera_tr.add_translation(camera_tr.get_forward_direction() * delta_time * speed);
			}

			// key pressed s
			if (keyboard.is_event(h5::keyboard_device::key::s, h5::input_device::event::repeated))
			{
				// move camera actor matrix backward
				camera_tr.add_translation(-camera_tr.get_forward_direction() * delta_time * speed);
			}

			// key pressed d
			if (keyboard.is_event(h5::keyboard_device::key::d, h5::input_device::event::repeated))
			{
				// move camera actor matrix right
				camera_tr.add_translation(camera_tr.get_right_direction() * delta_time * speed);
			}

			// key pressed a
			if (keyboard.is_event(h5::keyboard_device::key::a, h5::input_device::event::repeated))
			{
				// move camera actor matrix left
				camera_tr.add_translation(-camera_tr.get_right_direction() * delta_time * speed);
			}

			// key pressed space
			if (keyboard.is_event(h5::keyboard_device::key::space, h5::input_device::event::repeated))
			{
				// move camera actor matrix up
				camera_tr.add_translation(camera_tr.get_up_direction() * delta_time * speed);
			}

			// key pressed ctrl
			if (keyboard.is_event(h5::keyboard_device::key::lcontrol, h5::input_device::event::repeated))
			{
				// move camera actor matrix down
				camera_tr.add_translation(-camera_tr.get_up_direction() * delta_time * speed);
			}
			// key pressed escape
			if (keyboard.is_event(h5::keyboard_device::key::escape, h5::input_device::event::pressed))
			{
				// break main loop
				break;
			}

			// rotate cube
			if (keyboard.is_event(h5::keyboard_device::key::up, h5::input_device::event::repeated))
			{
				obj_tr.add_rotation(
					h5::quat(
						obj_tr.get_right_direction(),
						deg_to_rad(delta_time) * 90.0f
					)
				);
			}

			// rotate cube
			if (keyboard.is_event(h5::keyboard_device::key::down, h5::input_device::event::repeated))
			{
				obj_tr.add_rotation(
					h5::quat(
						obj_tr.get_right_direction(),
						deg_to_rad(-delta_time) * 90.0f
					)
				);
			}

			// rotate cube
			if (keyboard.is_event(h5::keyboard_device::key::right, h5::input_device::event::repeated))
			{
				obj_tr.add_rotation(
					h5::quat(
						obj_tr.get_forward_direction(),
						deg_to_rad(-delta_time) * 90.0f
					)
				);
			}

			// rotate cube
			if (keyboard.is_event(h5::keyboard_device::key::left, h5::input_device::event::repeated))
			{
				obj_tr.add_rotation(
					h5::quat(
						obj_tr.get_forward_direction(),
						deg_to_rad(delta_time) * 90.0f
					)
				);
			}
		}

		if (!error_level)
		{
			// update camera vp
			camera_v.look_at(
				camera_tr.get_translation(),
				camera_tr.get_forward_direction() + camera_tr.get_translation(),
				camera_tr.get_up_direction()
			);
			camera_p.perspective(deg_to_rad(60.0f), 1280.0f / 720.0f, 0.001f, 10240.0f);
			camera_vp = camera_v * camera_p;

			// update final object matrix
			h5::matrix matrix = obj_tr.get_transform_matrix() * camera_vp;

			// clear viewport
			render.clear(h5::colorf_rgba::black);

			// draw cube			
			render.set_vertex_buffer(&vb);
			render.set_index_buffer(&ib);
			render.set_vertex_shader(&vs);
			render.set_pixel_shader(&ps);
			render.set_vertex_shader_constant_buffer(&cb, 0);
			render.update_constant_buffer(&cb, &matrix, 0);
			render.draw_indexed(indices.size());

			// text to draw
			wchar_t text[256]{};
			swprintf(text, 256, u("fps: %f delta: %f\nw, a, s, d - camera move\nq, e and mouse axis - camera rotate\nkeyboard arrows - cube rotate\nesc - exit"), 1.0f / delta_time, delta_time);

			// draw draw info text
			render.draw_text(
				&font,
				text,
				h5::vector2(10.0f, 10.0f),
				h5::colorf_rgba::white
			);

			// rendering
			error_level = render.present();
		}									
	}

	return error_level;
}
