#pragma once

#include "transform_component.h"

#include "static_mesh.h"

#include "physics.h"

namespace h5
{
	class physics_interface;
	class static_mesh;
	class scene;

	class physics_component
		: public transform_component
	{
	public:
		friend class physics_interface;

	public:
		struct construct_info
		{
			collision_object::shape_type		shape_type		= collision_object::shape_type::none;
			collision_object::collision_type	collision_type	= collision_object::collision_type::none;
			float								radius			= 1.0f;
			static_mesh							*mesh_ptr		= nullptr;
			float								mass			= 0.0f;

			construct_info() {}

			construct_info(collision_object::shape_type shape_type, float radius)
				: shape_type(shape_type)
				, radius(radius)
			{}

			construct_info(collision_object::shape_type shape_type, rigid_body::collision_type collision_type, static_mesh *mesh_ptr = nullptr, float mass = 0.0f)
				: shape_type(shape_type)
				, collision_type(collision_type)
				, mesh_ptr(mesh_ptr)
				, mass(mass)
			{}

			~construct_info() {}
		};

	protected:
		physics_world *physics_world_ptr = nullptr;

	public:
		physics_component();
		physics_component(const construct_info &info);
		virtual ~physics_component();

	public:
		virtual void set_translation(const vector3 &v) override;
		virtual void set_rotation(const quat &r) override;
		virtual void set_scale(const vector3 &v) override;

	public:
		virtual vector3 get_translation() const override;
		virtual quat get_rotation() const override;
		virtual vector3 get_scaling() const override;

	public:
		virtual void add_translation(const vector3 &v) override;
		virtual void add_rotation(const quat &q) override;
		virtual void add_scaling(const vector3 &v) override;

	public:
		virtual vector3 get_forward_direction() const override;
		virtual vector3 get_right_direction() const override;
		virtual vector3 get_up_direction() const override;

	public:
		virtual matrix get_translation_matrix() const override;
		virtual matrix get_rotation_matrix() const override;
		virtual matrix get_scaling_matrix() const override;
		virtual matrix get_transform_matrix() const override;

	public:
		virtual void on_insert_to_scene(scene *ptr) override;
		virtual void on_remove_from_scene(scene *ptr) override;

	public:
		void set_mass(float mass);
		float get_mass() const;
		void clear_forces();
		void set_linear_velocity(const vector3 &v);
		void set_angular_velocity(const vector3 &v);

	public:
		physics_world *get_physics_world();
	};
}
