#include "view_component.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(view_component);

	const float view_component::FOV_DEFAULT = 60.0f;

	view_component::view_component()
		: transform_component()
	{}

	view_component::~view_component() {}	

	matrix view_component::get_view_matrix()
	{
		const vector3 translation = get_translation();
		return view_matrix(
			translation,
			translation + get_forward_direction(),
			get_up_direction()
		);
	}

	float view_component::get_fov()
	{
		return fov;
	}
}
