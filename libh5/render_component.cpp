#include "render_component.h"

#include "meshes.h"

#include "game_engine.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(render_component);

	render_component::render_component()
		: transform_component()
	{}

	render_component::render_component(const construct_info &info)
		: transform_component()
		, mesh_ptr(info.mesh_ptr)
	{
		mesh_ptr->inc_ref();
	}

	render_component::~render_component()
	{
		mesh_ptr->dec_ref();
	}	

	static_mesh *render_component::get_mesh()
	{
		return mesh_ptr;
	}

	void render_component::on_insert_to_scene(scene *ptr)
	{
		transform_component::on_insert_to_scene(ptr);

		render_interface::get_instance()->insert_component(this);
	}

	void render_component::on_remove_from_scene(scene *ptr)
	{
		transform_component::on_remove_from_scene(ptr);

		render_interface::get_instance()->remove_component(this);
	}
}
