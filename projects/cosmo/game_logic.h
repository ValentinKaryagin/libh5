#pragma once

#include "..\..\libh5\h5.h"

namespace cosmo
{
	class game_logic
		: public h5::game_logic
	{
	protected:
		game_logic();

	public:
		virtual ~game_logic();

	public:
		int32_t init() override;

	public:
		static game_logic *get_instance();
	};
}
