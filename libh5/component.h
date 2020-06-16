#pragma once

#include "object.h"

namespace h5
{
	class component
		: public object
	{
	public:
		component();
		virtual ~component();

	public:
		virtual void tick(float delta_seconds);
	};
}
