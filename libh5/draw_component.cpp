#include "draw_component.h"

#include "render_interface.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(draw_component);

	draw_component::draw_component()
		: render_component()
	{}

	draw_component::draw_component(const construct_info &info)
		: render_component(info)
	{}

	draw_component::~draw_component() {}

	int32_t draw_component::on_draw(render_interface *ptr)
	{
		return ptr->draw(this);
	}
}
