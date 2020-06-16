#pragma once

#include "render_component.h"

namespace h5
{
	class draw_component
		: public render_component
	{
	public:
		draw_component();
		draw_component(const construct_info &info);
		virtual ~draw_component();

	public:
		int32_t on_draw(render_interface *ptr) override;
	};
}
