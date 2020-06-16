#include "physics_component.h"

#include "static_mesh.h"

#include "game_engine.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(physics_component);

	physics_component::physics_component()
		: transform_component()
	{}

	physics_component::physics_component(const construct_info &info)
		: transform_component()
	{
		/*std::vector<vector3> vertices;
		if (mesh_ptr)
		{
			for (auto &item : mesh_ptr->vertices)
			{
				vertices.push_back(item.position);
			}
		}

		_collision_object = new rigid_body(
			rigid_body::construct_info(
				info.shape_type,
				info.collision_type,
				info.radius,
				info.mesh_ptr ? &vertices : nullptr,
				info.mesh_ptr ? &info.mesh_ptr->indices : nullptr,
				info.mass));*/
	}

	physics_component::~physics_component() {}

	void physics_component::set_translation(const vector3 &v)
	{
		_collision_object->set_translation(v);
	}

	void physics_component::set_rotation(const quat &r)
	{
		_collision_object->set_rotation(r);
	}

	void physics_component::set_scale(const vector3 &v)
	{
		// do nothing
	}

	vector3 physics_component::get_translation() const
	{
		return _collision_object->get_translation();
	}

	quat physics_component::get_rotation() const
	{
		return _collision_object->get_rotation();
	}

	vector3 physics_component::get_scaling() const
	{
		return vector3::identity;
	}

	void physics_component::add_translation(const vector3 &v)
	{

	}

	void physics_component::add_rotation(const quat &q)
	{

	}

	void physics_component::add_scaling(const vector3 &v)
	{

	}

	vector3 physics_component::get_forward_direction() const
	{
		return vector3();
	}

	vector3 physics_component::get_right_direction() const
	{
		return vector3();
	}

	vector3 physics_component::get_up_direction() const
	{
		return vector3();
	}

	matrix physics_component::get_translation_matrix() const
	{
		return matrix();
	}

	matrix physics_component::get_rotation_matrix() const
	{
		return matrix();
	}

	matrix physics_component::get_scaling_matrix() const
	{
		return matrix::identity;
	}

	matrix physics_component::get_transform_matrix() const
	{
		return _collision_object->get_transform_matrix();
	}

	void physics_component::on_insert_to_scene(scene *ptr)
	{
		transform_component::on_insert_to_scene(ptr);

		physics_interface::get_instance()->insert_component(this);
	}

	void physics_component::on_remove_from_scene(scene *ptr)
	{
		transform_component::on_remove_from_scene(ptr);

		physics_interface::get_instance()->remove_component(this);
	}		

	void physics_component::set_mass(float mass)
	{
		_collision_object->set_mass(mass);
	}

	float physics_component::get_mass() const
	{
		return _collision_object->get_mass();
	}

	void physics_component::clear_forces()
	{
		_collision_object->clear_forces();
	}

	void physics_component::set_linear_velocity(const vector3 &v)
	{
		_collision_object->set_linear_velocity(v);
	}

	void physics_component::set_angular_velocity(const vector3 &v)
	{
		_collision_object->set_angular_velocity(v);
	}

	physics_world *physics_component::get_physics_world()
	{
		return physics_world_ptr;
	}
}
