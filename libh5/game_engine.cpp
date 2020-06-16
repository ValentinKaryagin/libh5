#include "game_engine.h"
#include "game_logic.h"

#include <ctime>

namespace h5
{
	game_engine::game_engine()
		: object()
	{
		DEBUG_PRINT_FUNC();
	}

	game_engine::~game_engine()
	{
		DEBUG_PRINT_FUNC();

		terminate();
	}

	int32_t game_engine::init()
	{
		DEBUG_PRINT_FUNC();

		int32_t error_level = 0;
		
		if (!error_level)
		{
			error_level = application_interface::get_instance()->init_threaded();
		}		

		if (!error_level)
		{
			error_level = input_interface::get_instance()->init_threaded();
		}

		if (!error_level)
		{
			error_level = render_interface::get_instance()->init();
		}

		if (!error_level)
		{
			error_level = physics_interface::get_instance()->init();
		}		

		/*if (!error_level)
		{
			error_level = render.create_sprite_font(u("consolas14.spritefont"), &font);
		}*/

		return error_level;
	}

	void game_engine::terminate()
	{
		DEBUG_PRINT_FUNC();	

		physics_interface::get_instance()->terminate();

		render_interface::get_instance()->terminate();		

		input_interface::get_instance()->terminate_threaded();
		application_interface::get_instance()->terminate_threaded();
	}

	void game_engine::set_logic(game_logic *ptr)
	{
		logic_ptr = ptr;
	}

	int32_t game_engine::loop()
	{
		DEBUG_PRINT_FUNC();

		int32_t error_level = 0;

		if (!logic_ptr)
		{
			error_level = ERRLVL_GF_GAME_LOGIC_NOT_DEFINED;
		}

		if (!error_level)
		{
			error_level = logic_ptr->init();
		}

		if (!error_level)
		{
			time_t pre = clock(), post = 0;
			float delta_time = 0;

			while (!error_level && !is_exit_requested)
			{
				post = clock();
				delta_time = static_cast<float>(post - pre) * 0.001f;
				pre = post;

				if (!error_level)
				{
					logic_ptr->tick(delta_time);
				}

				if (!error_level)
				{
					error_level = physics_interface::get_instance()->simulate(delta_time);
				}

				if (!error_level)
				{
					error_level = render_interface::get_instance()->draw();
				}

				/*if (!error_level)
				{
					error_level = window.proc();
				}

				if (!error_level)
				{
					error_level = keyboard.read();
				}

				if (!error_level)
				{
					error_level = mouse.read();
				}

				if (!error_level)
				{
					if (logic)
					{
						error_level = logic->tick(delta_time);
					}
				}

				if (!error_level)
				{
					error_level = physics.simulate(delta_time);
				}

				if (!error_level)
				{
					render.clear(colorf_rgba::black);

					if (is_draw_info_text)
					{
						if (ticks_count % info_text_update_rate == 0)
						{
							swprintf(info_text_buffer, INFO_TEXT_BUFFER_SIZE, u("fps: %f delta: %f ticks: %llu time: %f\n"), 1.0f / delta_time, delta_time, ticks_count, time_passed_when_started);
						}

						render.draw_text(
							&font,
							info_text_buffer,
							info_text_coords,
							info_text_color
						);
					}

					projection.perspective(fov, window.get_aspect_ratio(), near_plane, far_plane);

					// todo: draw objects

					error_level = render.present();
				}*/

				++ticks_count;

				time_passed_when_started += delta_time;
			}
		}	

		if (logic_ptr)
		{
			logic_ptr->terminate();
		}	

		return error_level;
	}

	game_engine *game_engine::get_instance()
	{
		static game_engine game_engine_instance;
		return &game_engine_instance;
	}
}
