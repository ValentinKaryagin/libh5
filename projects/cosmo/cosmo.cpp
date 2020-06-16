#include "..\..\libh5\h5.h"

#include "game_logic.h"

#include <locale.h>

int main()
{
	setlocale(LC_ALL, "Russian");

	int32_t error_level = 0;

	h5::game_engine *game_engine = h5::game_engine::get_instance();

	error_level = game_engine->init();

	if (!error_level)
	{
		game_engine->set_logic(cosmo::game_logic::get_instance());
	}

	if (!error_level)
	{
		error_level = game_engine->loop();
	}

	return error_level;
}
 