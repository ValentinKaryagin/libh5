#include "..\..\libh5\h5.h"

//#include "..\\..\\third_party\\DirectXMesh-aug2019\\Utilities\\WaveFrontReader.h"

//#include <vector>
//#include <iostream>
//#include <ctime>

//#define MAX_STR_LEN 256

//const wchar_t info_tpl[] = u("fps: %f delta: %f\nw, a, s, d - camera move\nq, e and mouse axis - camera rotate\nkeyboard arrows - object rotate\nesc - exit");

int main()
{
	h5::mesh mesh(u(".\\cube.obj"));
	mesh.init();

	return 0;

	/*
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

	WaveFrontReader<uint32_t> OBJFile;
	OBJFile.Load(L".\\plane200x200.obj");

	// vertices buffer
	h5::buffer vb;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::vertex, OBJFile.vertices.data(), OBJFile.vertices.size(), sizeof(h5::vertex), &vb);
	}

	// indices buffer
	h5::buffer ib;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::index, OBJFile.indices.data(), OBJFile.indices.size(), sizeof(uint32_t), &ib);
	}

	// vertex shader
	h5::vertex_shader vs;
	if (!error_level)
	{
		error_level = vs.load(u("experimental.hlsl"), "vs");
		if (!error_level)
		{
			error_level = render.create_vertex_shader(&vs);
		}
	}

	// pixel shader
	h5::pixel_shader ps;
	if (!error_level)
	{
		error_level = ps.load(u("experimental.hlsl"), "ps");
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

	const float camera_move_speed = 10.0f;
	const float mouse_sensevity = 10.0f;
	const float object_rotation_speed = 90.0f;

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
					deg_to_rad(static_cast<float>(mouse.get_axis(h5::mouse_device::axis::x)) * delta_time * mouse_sensevity)
				)
			);

			// add y rotation
			camera_tr.add_rotation(
				h5::quat(
					camera_tr.get_right_direction(),
					deg_to_rad(static_cast<float>(mouse.get_axis(h5::mouse_device::axis::y)) * delta_time * mouse_sensevity)
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
				camera_tr.add_translation(camera_tr.get_forward_direction() * delta_time * camera_move_speed);
			}

			// key pressed s
			if (keyboard.is_event(h5::keyboard_device::key::s, h5::input_device::event::repeated))
			{
				// move camera actor matrix backward
				camera_tr.add_translation(-camera_tr.get_forward_direction() * delta_time * camera_move_speed);
			}

			// key pressed d
			if (keyboard.is_event(h5::keyboard_device::key::d, h5::input_device::event::repeated))
			{
				// move camera actor matrix right
				camera_tr.add_translation(camera_tr.get_right_direction() * delta_time * camera_move_speed);
			}

			// key pressed a
			if (keyboard.is_event(h5::keyboard_device::key::a, h5::input_device::event::repeated))
			{
				// move camera actor matrix left
				camera_tr.add_translation(-camera_tr.get_right_direction() * delta_time * camera_move_speed);
			}

			// key pressed space
			if (keyboard.is_event(h5::keyboard_device::key::space, h5::input_device::event::repeated))
			{
				// move camera actor matrix up
				camera_tr.add_translation(camera_tr.get_up_direction() * delta_time * camera_move_speed);
			}

			// key pressed ctrl
			if (keyboard.is_event(h5::keyboard_device::key::lcontrol, h5::input_device::event::repeated))
			{
				// move camera actor matrix down
				camera_tr.add_translation(-camera_tr.get_up_direction() * delta_time * camera_move_speed);
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
						deg_to_rad(delta_time) * object_rotation_speed
					)
				);
			}

			// rotate cube
			if (keyboard.is_event(h5::keyboard_device::key::down, h5::input_device::event::repeated))
			{
				obj_tr.add_rotation(
					h5::quat(
						obj_tr.get_right_direction(),
						deg_to_rad(-delta_time) * object_rotation_speed
					)
				);
			}

			// rotate cube
			if (keyboard.is_event(h5::keyboard_device::key::right, h5::input_device::event::repeated))
			{
				obj_tr.add_rotation(
					h5::quat(
						obj_tr.get_forward_direction(),
						deg_to_rad(-delta_time) * object_rotation_speed
					)
				);
			}

			// rotate cube
			if (keyboard.is_event(h5::keyboard_device::key::left, h5::input_device::event::repeated))
			{
				obj_tr.add_rotation(
					h5::quat(
						obj_tr.get_forward_direction(),
						deg_to_rad(delta_time) *object_rotation_speed
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
			camera_p.perspective(deg_to_rad(60.0f), window.get_aspect_ratio(), 0.001f, 10240.0f);
			camera_vp = camera_v * camera_p;

			// update final object matrix
			h5::matrix matrix = obj_tr.get_transform_matrix() * camera_vp;

			// clear viewport
			render.clear(h5::colorf_rgba::black);

			// wireframe render state
			render.set_render_state(h5::render_device::state::wireframe);

			// draw cube			
			render.set_vertex_buffer(&vb);
			render.set_index_buffer(&ib);
			render.set_vertex_shader(&vs);
			render.set_pixel_shader(&ps);
			render.set_vertex_shader_constant_buffer(&cb, 0);
			render.update_constant_buffer(&cb, &matrix, 0);
			render.draw_indexed(OBJFile.indices.size());
			
			// text to draw
			wchar_t info_text[MAX_STR_LEN]{};
			swprintf(info_text, MAX_STR_LEN, info_tpl, 1.0f / delta_time, delta_time);

			// draw draw info text
			render.draw_text(
				&font,
				info_text,
				h5::vector2(10.0f, 10.0f),
				h5::colorf_rgba::white
			);

			// rendering
			error_level = render.present();
		}
	}

	OBJFile.Clear();

	return error_level;*/
}

//#include "..\..\libh5\h5.h"

//#include <ctime>
//#include <string>
//#include <vector>

//#include <conio.h>

//constexpr size_t OBJECTS_COUNT = 1000;

/*#include "..\\..\\third_party\DirectXMesh-aug2019\Utilities\WaveFrontReader.h"

#include "..\..\libh5\h5.h"

#include <vector>
#include <iostream>

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
		error_level = render.init(&window);
		render.set_render_state(h5::render_device::state::wireframe);
	}

	WaveFrontReader<uint32_t> OBJFile;
	OBJFile.Load(L".\\plane200x200.obj");

	// vertices buffer
	h5::buffer vb;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::vertex, OBJFile.vertices.data(), OBJFile.vertices.size(), sizeof(h5::vertex), &vb);
		if (!error_level)
		{
			render.set_vertex_buffer(&vb);
		}
	}

	// indices buffer
	h5::buffer ib;
	if (!error_level)
	{
		error_level = render.create_buffer(h5::buffer::type::index, OBJFile.indices.data(), OBJFile.indices.size(), sizeof(uint32_t), &ib);
		if (!error_level)
		{
			render.set_index_buffer(&ib);
		}
	}

	// vertex shader
	h5::vertex_shader vs;
	if (!error_level)
	{
		error_level = vs.load(u("experimental.hlsl"), "vs");
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
		error_level = ps.load(u("experimental.hlsl"), "ps");
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
	v.look_at(h5::vector3(-100.0f, 0.0f, 5.0f), h5::vector3(0.0f, 0.0f, 5.0f), h5::vector3::up);
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
			render.draw_indexed(OBJFile.indices.size());

			// rendering
			error_level = render.present();
		}
	}

	OBJFile.Clear();

	return error_level;
}*/

//int main()
//{
	/*h5::object *obj = h5::object::create<h5::view_component>();
	h5::object::destroy(&obj);

	return 0;*/

	/*int32_t error_level = 0;

	h5::game_engine *game_engine = h5::game_engine::get_instance();
	error_level = game_engine->init(nullptr);
	if (!error_level)
	{
		error_level = game_engine->loop();
	}

	return error_level;*/

	/*
	// result
	int32_t error_level = 0;

	// window
	h5::window window;
	error_level = window.init(128, 128, 1280, 720, u("h5 experimental"), nullptr, true, true);

	// steam api
	h5::steam_api *steam_api = h5::steam_api::get_instance();
	if (!error_level)
	{
		error_level = steam_api->init();
	}	

	// render
	h5::render_device render_device;
	if (!error_level)
	{
		error_level = render_device.init(&window, false, true);
	}

	if (!error_level)
	{
		while (!window.is_exit_requested())
		{
			// window events processing
			error_level = window.proc();
			if (error_level)
			{
				break;
			}

			// clear viewport
			render_device.clear(h5::colorf_rgba::black);

			// rendering
			error_level = render_device.present();
			if (error_level)
			{
				break;
			}
		}
	}

	return error_level;
	*/

	/*h5::guid g;
	printf("%s\n", g.to_string().c_str());
	wprintf(u("%s\n"), g.to_wstring().c_str());

	return 0;*/

	/*h5::iocp_server server;
	if (!server.init(7777, "127.0.0.1"))
	{
		return 0;
	}

	while (_getch() != VK_ESCAPE);

	return 0;*/

	/*srand(time(nullptr));

	unsigned char key[596]{};
	for (size_t i = 0; i < 596; ++i)
	{
		key[i] = rand();
	}

	wchar_t enc[1024]{};
	DWORD enclen = 1024;
	CryptBinaryToStringW(key, 596, CRYPT_STRING_BASE64, enc, &enclen);

	unsigned char dec[596]{};
	DWORD declen = 596;

	CryptStringToBinaryW(enc, enclen, CRYPT_STRING_BASE64, dec, &declen, nullptr, nullptr);

	return 0;*/	

	/*h5::stream_vector sv;
	sv.write_double(3.14159265);

	double f = 3.14159265;
	uint8_t bytes[8];
	memcpy(bytes, &f, 8);

	double v = sv.read_double();

	return 0;*/

	/*int32_t res = 0;

	h5::guid id;
	printf("%s", id.to_string().c_str());

	return res;*/
	/*h5::rsa rsa;
	rsa.init();

	std::string private_key = rsa.get_private_key();
	std::string public_key = rsa.get_public_key();

	rsa.terminate();

	std::srand(static_cast<uint32_t>(std::time(nullptr)));

	std::wstring wnd_title = TEXT("h5 experimental");

	std::vector<h5::vertex> vertices =
	{
		h5::vertex(h5::vector3(-1.0f, -1.0f, -1.0f), h5::vector2(0.0f, 1.0f), h5::vector3(-1.0f, -1.0f, -1.0f)),
		h5::vertex(h5::vector3(-1.0f, +1.0f, -1.0f), h5::vector2(0.0f, 0.0f), h5::vector3(-1.0f, +1.0f, -1.0f)),
		h5::vertex(h5::vector3(+1.0f, +1.0f, -1.0f), h5::vector2(1.0f, 0.0f), h5::vector3(+1.0f, +1.0f, -1.0f)),
		h5::vertex(h5::vector3(+1.0f, -1.0f, -1.0f), h5::vector2(1.0f, 1.0f), h5::vector3(+1.0f, -1.0f, -1.0f)),
		h5::vertex(h5::vector3(-1.0f, -1.0f, +1.0f), h5::vector2(1.0f, 1.0f), h5::vector3(-1.0f, -1.0f, +1.0f)),
		h5::vertex(h5::vector3(+1.0f, -1.0f, +1.0f), h5::vector2(0.0f, 1.0f), h5::vector3(+1.0f, -1.0f, +1.0f)),
		h5::vertex(h5::vector3(+1.0f, +1.0f, +1.0f), h5::vector2(0.0f, 0.0f), h5::vector3(+1.0f, +1.0f, +1.0f)),
		h5::vertex(h5::vector3(-1.0f, +1.0f, +1.0f), h5::vector2(1.0f, 0.0f), h5::vector3(-1.0f, +1.0f, +1.0f)),
		h5::vertex(h5::vector3(-1.0f, +1.0f, -1.0f), h5::vector2(0.0f, 1.0f), h5::vector3(-1.0f, +1.0f, -1.0f)),
		h5::vertex(h5::vector3(-1.0f, +1.0f, +1.0f), h5::vector2(0.0f, 0.0f), h5::vector3(-1.0f, +1.0f, +1.0f)),
		h5::vertex(h5::vector3(+1.0f, +1.0f, +1.0f), h5::vector2(1.0f, 0.0f), h5::vector3(+1.0f, +1.0f, +1.0f)),
		h5::vertex(h5::vector3(+1.0f, +1.0f, -1.0f), h5::vector2(1.0f, 1.0f), h5::vector3(+1.0f, +1.0f, -1.0f)),
		h5::vertex(h5::vector3(-1.0f, -1.0f, -1.0f), h5::vector2(1.0f, 1.0f), h5::vector3(-1.0f, -1.0f, -1.0f)),
		h5::vertex(h5::vector3(+1.0f, -1.0f, -1.0f), h5::vector2(0.0f, 1.0f), h5::vector3(+1.0f, -1.0f, -1.0f)),
		h5::vertex(h5::vector3(+1.0f, -1.0f, +1.0f), h5::vector2(0.0f, 0.0f), h5::vector3(+1.0f, -1.0f, +1.0f)),
		h5::vertex(h5::vector3(-1.0f, -1.0f, +1.0f), h5::vector2(1.0f, 0.0f), h5::vector3(-1.0f, -1.0f, +1.0f)),
		h5::vertex(h5::vector3(-1.0f, -1.0f, +1.0f), h5::vector2(0.0f, 1.0f), h5::vector3(-1.0f, -1.0f, +1.0f)),
		h5::vertex(h5::vector3(-1.0f, +1.0f, +1.0f), h5::vector2(0.0f, 0.0f), h5::vector3(-1.0f, +1.0f, +1.0f)),
		h5::vertex(h5::vector3(-1.0f, +1.0f, -1.0f), h5::vector2(1.0f, 0.0f), h5::vector3(-1.0f, +1.0f, -1.0f)),
		h5::vertex(h5::vector3(-1.0f, -1.0f, -1.0f), h5::vector2(1.0f, 1.0f), h5::vector3(-1.0f, -1.0f, -1.0f)),
		h5::vertex(h5::vector3(+1.0f, -1.0f, -1.0f), h5::vector2(0.0f, 1.0f), h5::vector3(+1.0f, -1.0f, -1.0f)),
		h5::vertex(h5::vector3(+1.0f, +1.0f, -1.0f), h5::vector2(0.0f, 0.0f), h5::vector3(+1.0f, +1.0f, -1.0f)),
		h5::vertex(h5::vector3(+1.0f, +1.0f, +1.0f), h5::vector2(1.0f, 0.0f), h5::vector3(+1.0f, +1.0f, +1.0f)),
		h5::vertex(h5::vector3(+1.0f, -1.0f, +1.0f), h5::vector2(1.0f, 1.0f), h5::vector3(+1.0f, -1.0f, +1.0f))
	};

	std::vector<uint32_t> indices =
	{ 0, 1, 2, 0, 2, 3,	4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 20, 21, 22, 20, 22, 23 };

	h5::window window;
	error_level = window.init(128, 128, 1280, 720, wnd_title.c_str());
	assert(!error_level);

	h5::keyboard keyboard;
	error_level = keyboard.init(&window);
	assert(!error_level);

	h5::mouse mouse;
	error_level = mouse.init(&window);
	assert(!error_level);

	h5::render_device render_device;
	error_level = render_device.init(&window, false, true);
	assert(!error_level);

	render_device.set_render_state(h5::render_device::state::solid);

	h5::buffer vb;
	error_level = render_device.create_buffer(h5::buffer::type::vertex, vertices.data(), vertices.size(), sizeof(h5::vertex), &vb);
	assert(!error_level);
	render_device.set_vertex_buffer(&vb);

	h5::buffer ib;
	error_level = render_device.create_buffer(h5::buffer::type::index, indices.data(), indices.size(), sizeof(uint32_t), &ib);
	assert(!error_level);
	render_device.set_index_buffer(&ib);

	h5::vertex_shader vs;
	error_level = vs.compile_from_file(TEXT("experimental.hlsl"), "vs");
	assert(!error_level);
	error_level = render_device.create_vertex_shader(&vs);
	assert(!error_level);
	render_device.set_vertex_shader(&vs);

	h5::pixel_shader ps;
	error_level = ps.compile_from_file(TEXT("experimental.hlsl"), "ps");
	assert(!error_level);
	error_level = render_device.create_pixel_shader(&ps);
	assert(!error_level);
	render_device.set_pixel_shader(&ps);

	h5::matrix v, p, vp, wvp;
	v.look_at(h5::vector3(-40.0f, 10.0f, 10.0f), h5::vector3::zero, h5::vector3::up);
	p.perspective(deg_to_rad(60.0f), 1280.0f / 720.0f, 1.0f, 10240.0f);
	vp = v * p;

	std::vector<h5::matrix> w_matrices;
	for (size_t i = 0; i < OBJECTS_COUNT; ++i)
	{
		h5::matrix w;
		w.translate(h5::vector3(
			static_cast<float>(std::rand() % 40 - 20),
			static_cast<float>(std::rand() % 40 - 20),
			static_cast<float>(std::rand() % 40 - 20)));
		w_matrices.push_back(w);
	}
	
	h5::buffer cb;
	error_level = render_device.create_buffer(h5::buffer::type::constant, &vp, 1, sizeof(h5::matrix), &cb);
	assert(!error_level);
	render_device.set_vertex_shader_constant_buffer(&cb, 0);

	h5::buffer srb;
	error_level = render_device.create_buffer(h5::buffer::type::structured_buffer, w_matrices.data(), OBJECTS_COUNT, sizeof(h5::matrix), &srb);
	assert(!error_level);

	h5::shader_res sr;
	error_level = render_device.create_shader_res(&srb, OBJECTS_COUNT, &sr);
	assert(!error_level);
	render_device.set_vertex_shader_res(&sr, 0);

	h5::texture2d texture_color;
	error_level = texture_color.load(TEXT("grass_clr.png"));
	assert(!error_level);*/

	/*h5::texture2d texture_normal;
	error_level = texture_normal.load(TEXT("grass_nrm.png"));
	assert(!error_level);*/

	/*h5::shader_res texture_color_sr;
	error_level = render_device.create_shader_res(&texture_color, &texture_color_sr);
	assert(!error_level);
	render_device.set_pixel_shader_res(&texture_color_sr, 0);*/

	/*h5::shader_res texture_normal_sr;
	error_level = render_device.create_shader_res(&texture_normal, &texture_normal_sr);
	assert(!error_level);
	render_device.set_pixel_shader_res(&texture_normal_sr, 1);*/

	/*size_t frames = 0;

	clock_t pre = clock(), post = 0;

	while (!window.is_exit_requested())
	{
		window.proc();

		keyboard.read();
		mouse.read();*/

		/*if (keyboard.is_event(h5::keyboard::key::w, h5::input_device::event::pressed))
		{
			printf("w pressed!\n");
		}
		if (keyboard.is_event(h5::keyboard::key::w, h5::input_device::event::repeated))
		{
			printf("w repeated!\n");
		}
		if (keyboard.is_event(h5::keyboard::key::w, h5::input_device::event::released))
		{
			printf("w released!\n");
		}

		if (mouse.is_event(h5::mouse::button::mb0, h5::input_device::event::pressed))
		{
			printf("mb0 pressed!\n");
		}
		if (mouse.is_event(h5::mouse::button::mb0, h5::input_device::event::repeated))
		{
			printf("mb0 repeated!\n");
		}
		if (mouse.is_event(h5::mouse::button::mb0, h5::input_device::event::released))
		{
			printf("mb0 released!\n");
		}

		printf(
			"mouse_axis: %d %d %d\n",
			mouse.get_axis(h5::mouse::axis::x),
			mouse.get_axis(h5::mouse::axis::y),
			mouse.get_axis(h5::mouse::axis::z));*/

		//render_device.clear(&h5::colorf_rgba::black);

		/*for (auto &item : w_matrices)
		{
			wvp = item * vp;

			render_device.update_constant_buffer(&cb, &wvp);

			render_device.set_vertex_shader_constant_buffer(&cb);
			render_device.set_pixel_shader_constant_buffer(&cb);

			render_device.draw_indexed(indices.size());
		}*/
		
		/*render_device.draw_indexed_instanced(indices.size(), OBJECTS_COUNT);

		error_level = render_device.present();
		assert(!error_level);

		post = clock();

		if (frames % 10 == 0)
		{
			const float frame_time = (post - pre) * 0.001f;
			window.set_title(
				wnd_title
				+ TEXT(" - ft: ") + std::to_wstring(frame_time)
				+ TEXT(" fps: ") + std::to_wstring(1.0f / frame_time)
#ifdef _DEBUG
				+ TEXT(" objs: ") + std::to_wstring(render_device.get_objects_to_draw_count())
				+ TEXT(" dc: ") + std::to_wstring(render_device.get_draw_calls_count())
#endif
			);
		}
		
		pre = post;
		post = 0;

		++frames;
	}*/

	//return error_level;
//}
