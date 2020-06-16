#include "game_logic.h"

#include "scene.h"

namespace h5
{
	game_logic::game_logic()
		: object()
	{
		DEBUG_PRINT_FUNC();
	}

	game_logic::~game_logic()
	{
		DEBUG_PRINT_FUNC();

		terminate();
	}

	int32_t game_logic::init()
	{
		DEBUG_PRINT_FUNC();

		int32_t error_level = 0;

		if (scene)
		{
			error_level = scene->init();
		}		

		return error_level;
	}

	void game_logic::terminate()
	{
		DEBUG_PRINT_FUNC();

		if (scene)
		{
			scene->terminate();
		}
	}

	int32_t game_logic::tick(float dt)
	{
		int32_t error_level = 0;

		return error_level;
	}

	game_logic *game_logic::get_instance()
	{
		static game_logic game_logic_instance;
		return &game_logic_instance;
	}
}
