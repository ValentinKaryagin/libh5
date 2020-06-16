#pragma once

#include "object.h"
#include "interfaces.h"

#define INFO_TEXT_BUFFER_SIZE				256
#define INFO_TEXT_UPDATE_RATE_DEFAULT_VALUE	10
#define INFO_TEXT_COORDS_DEFAULT_VALUE		h5::vector2(10.0f, 10.0f)

namespace h5
{
	class game_logic;

	class game_engine
		: public object
	{
	public:
		volatile bool is_exit_requested = false;

	protected:
		//sprite_font font;

	public:
		//bool is_draw_info_text = true;

	protected:
		/*wchar_t	info_text_buffer[INFO_TEXT_BUFFER_SIZE]{};
		size_t	info_text_update_rate	= INFO_TEXT_UPDATE_RATE_DEFAULT_VALUE;
		vector2	info_text_coords		= INFO_TEXT_COORDS_DEFAULT_VALUE;
		colorf_rgba info_text_color		= colorf_rgba::white;*/

	protected:
		size_t ticks_count = 0;
		float time_passed_when_started = 0.0f;

	protected:
		game_logic *logic_ptr = nullptr;

	protected:
		game_engine();

	public:
		virtual ~game_engine();

	public:
		int32_t init();
		void terminate();

	public:
		void set_logic(game_logic *ptr);

	public:
		int32_t loop();	

	public:
		static game_engine *get_instance();
	};
}
