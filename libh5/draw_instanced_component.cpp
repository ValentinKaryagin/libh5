#include "draw_instanced_component.h"

#include "render_interface.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(draw_instanced_component);

	draw_instanced_component::draw_instanced_component()
		: render_component()
	{}

	draw_instanced_component::draw_instanced_component(const construct_info &info)
		: render_component(info)
	{}

	draw_instanced_component::~draw_instanced_component() {}

	int32_t draw_instanced_component::on_draw(render_interface *ptr)
	{
		return ptr->draw(this);
	}

	std::vector<matrix> draw_instanced_component::get_transform_matrices()
	{
		std::vector<matrix> result;
		matrix transform_matrix = get_transform_matrix();
		const size_t trs_count = components.size();
		for (size_t i = 0; i < trs_count; ++i)
		{
			result.push_back(transform_matrix * components[i].get_transform_matrix());
		}
		return result;
	}
}
