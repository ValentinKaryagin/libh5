#include "scene.h"

namespace cosmo
{
	CREATE_ALLOCATOR_POOL(scene);

	scene::scene()
		: h5::scene()
	{}

	scene::~scene() {}

	int32_t scene::init()
	{
		int32_t error_level = 0;
		
		if (!error_level)
		{
			error_level = h5::scene::init();
		}
		
		if (!error_level)
		{
			// plane
			plane_mesh = create<h5::plane200x200_mesh>();
						
			h5::physics_component *pc = create<h5::physics_component>(
				h5::physics_component::construct_info(
					h5::collision_object::shape_type::triangle_mesh,
					h5::rigid_body::collision_type::statical,
					plane_mesh));
			pc->set_translation(h5::vector3(50.0f, 0.0f, -5.0f));
			insert_component(pc);

			h5::draw_component *dc = create<h5::draw_component>(h5::draw_component::construct_info(plane_mesh));
			dc->attach_to(pc);
			insert_component(dc);

			plane_physics.push_back(pc);
			plane_draws.push_back(dc);

			// cubes
			obj_mesh = create<h5::cube_mesh>();

			for (size_t i = 0; i < 256; ++i)
			{
				h5::physics_component *pc = create<h5::physics_component>(
					h5::physics_component::construct_info(
						h5::collision_object::shape_type::convex_hull,
						h5::rigid_body::collision_type::dynamical,
						obj_mesh,
						1.0f));
				pc->set_translation(h5::vector3(50.0f, 0.0f, (float)i * 4.0f));
				insert_component(pc);

				h5::draw_component *dc = create<h5::draw_component>(h5::draw_component::construct_info(obj_mesh));
				dc->attach_to(pc);
				insert_component(dc);

				obj_draws.push_back(dc);
				obj_physics.push_back(pc);
			}
		}		

		return error_level;
	}
}
