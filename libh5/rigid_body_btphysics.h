#pragma once

#include "collision_object_btphysics.h"

namespace h5
{
	class static_mesh;

	class rigid_body_btphysics
		: public collision_object_btphysics
	{
	public:
		friend class physics_world_btphysics;

	public:
		struct construct_info
		{
			shape_type				_shape_type		= shape_type::none;
			collision_type			_collision_type	= collision_type::none;
			float					radius			= 1.0f;
			static_mesh				*mesh_ptr		= nullptr;
			float					mass			= 0.0f;

			construct_info() {}

			construct_info(collision_object_btphysics::shape_type shape_type, collision_object_btphysics::collision_type collision_type, static_mesh *mesh_ptr = nullptr, float mass = 0.0f)
				: _shape_type(shape_type)
				, _collision_type(collision_type)
				, mesh_ptr(mesh_ptr)
				, mass(mass)
			{}
		};

	protected:
		construct_info cinfo;

	protected:
		btRigidBody	*bt_rigid_body = nullptr;

	public:
		rigid_body_btphysics();
		rigid_body_btphysics(const construct_info &info);
		virtual ~rigid_body_btphysics();

	public:
		void set_mass(float mass) override;
		float get_mass() override;
		void clear_forces() override;
		void set_linear_velocity(const vector3 &v) override;
		void set_angular_velocity(const vector3 &v) override;

	public:
		void set_translation(const vector3 &v) override;
		void set_rotation(const quat &r) override;

	public:
		matrix get_transform_matrix() const override;
		vector3 get_translation() const override;
		quat get_rotation() const override;

	public:
		virtual void on_insert_to_physics_world(physics_world_btphysics *ptr) override;
		virtual void on_remove_from_physics_world(physics_world_btphysics *ptr) override;
	};
}
