#include "game_logic.h"

#include "scene.h"

namespace cosmo
{
	game_logic::game_logic()
		: h5::game_logic()
	{
		scene = create<cosmo::scene>();
	}

	game_logic::~game_logic() {}

	int32_t game_logic::init()
	{
		int32_t error_level = 0;

		if (!error_level)
		{
			error_level = h5::game_logic::init();
		}		
		
		return error_level;
	}

	game_logic *game_logic::get_instance()
	{
		static game_logic game_logic_instance;
		return &game_logic_instance;
	}
}
