#pragma once

#include "render_component.h"

namespace h5
{
	class draw_instanced_component
		: public render_component
	{
	public:
		friend class render_interface;

	protected:
		std::vector<transform_component> components;

	public:
		draw_instanced_component();
		draw_instanced_component(const construct_info &info);
		virtual ~draw_instanced_component();

	public:
		int32_t on_draw(render_interface *ptr) override;

	public:
		std::vector<matrix> get_transform_matrices();
	};
}
