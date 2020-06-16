#include "transform_component.h"

#include "scene.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(transform_component);

	transform_component::transform_component()
		: component()
		, translation(vector3::zero)
		, rotation(quat::identity)
		, scaling(vector3::identity)
	{}

	transform_component::~transform_component() {}

	void transform_component::attach(transform_component *ptr)
	{
		ptr->root = this;
	}

	void transform_component::attach_to(transform_component *ptr)
	{
		root = ptr;
	}

	void transform_component::detach(transform_component *ptr)
	{
		ptr->root = nullptr;
	}

	void transform_component::detach()
	{
		root = nullptr;
	}

	void transform_component::set_translation(const vector3 &v)
	{
		translation = v;
	}

	void transform_component::set_rotation(const quat &r)
	{
		rotation = r;
	}

	void transform_component::set_scale(const vector3 &v)
	{
		scaling = v;
	}

	vector3 transform_component::get_translation() const
	{
		return translation;
	}

	quat transform_component::get_rotation() const
	{
		return rotation;
	}

	vector3 transform_component::get_scaling() const
	{
		return scaling;
	}

	void transform_component::add_translation(const vector3 &v)
	{
		translation += v;
	}

	void transform_component::add_rotation(const quat &q)
	{
		rotation = q * rotation;
	}

	void transform_component::add_scaling(const vector3 &v)
	{
		scaling += v;
	}

	vector3 transform_component::get_forward_direction() const
	{
		matrix transform_matrix = get_transform_matrix();
		return vector3(transform_matrix.m[0][0], transform_matrix.m[0][1], transform_matrix.m[0][2]);
	}

	vector3 transform_component::get_right_direction() const
	{
		matrix transform_matrix = get_transform_matrix();
		return vector3(transform_matrix.m[1][0], transform_matrix.m[1][1], transform_matrix.m[1][2]);
	}

	vector3 transform_component::get_up_direction() const
	{
		matrix transform_matrix = get_transform_matrix();
		return vector3(transform_matrix.m[2][0], transform_matrix.m[2][1], transform_matrix.m[2][2]);
	}

	matrix transform_component::get_translation_matrix() const
	{
		return translation_matrix(translation);
	}

	matrix transform_component::get_rotation_matrix() const
	{
		return rotation_matrix(rotation);
	}

	matrix transform_component::get_scaling_matrix() const
	{
		return scaling_matrix(scaling);
	}

	matrix transform_component::get_transform_matrix() const
	{
		matrix result = scaling_matrix(scaling) * rotation_matrix(rotation) * translation_matrix(translation);
		return root ? root->get_transform_matrix() * result : result;
	}

	void transform_component::on_insert_to_scene(scene *ptr)
	{
		ptr->insert(this);
		scene_ptr = ptr;
	}

	void transform_component::on_remove_from_scene(scene *ptr)
	{
		ptr->remove(this);
		scene_ptr = nullptr;
	}

	scene *transform_component::get_scene()
	{
		return scene_ptr;
	}
}
