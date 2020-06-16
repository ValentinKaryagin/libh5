#pragma once

#include "object.h"

namespace h5
{
	class scene;

	class game_logic
		: public object
	{
	protected:
		scene *scene = nullptr;

	protected:
		game_logic();

	public:
		virtual ~game_logic();

	public:
		virtual int32_t init();
		virtual void terminate();

	public:
		virtual int32_t tick(float dt);

	public:
		static game_logic *get_instance();
	};	
}
