#pragma once

#include "btphysics_public.h"
#include "math.h"

namespace h5
{
	class physics_world_btphysics;

	class collision_object_btphysics
	{
	public:
		enum class collision_type
		{
			none,
			statical,
			dynamical
		};

		enum class shape_type
		{
			none,
			sphere,
			convex_hull,
			triangle_mesh,
		};

	protected:
		btDefaultMotionState	*bt_motion_state		= nullptr;
		btTriangleMesh			*bt_triangle_mesh		= nullptr;
		btCollisionShape		*bt_collision_shape		= nullptr;

	public:
		collision_object_btphysics();
		virtual ~collision_object_btphysics();

	public:
		virtual void set_mass(float mass) = 0;
		virtual float get_mass() = 0;
		virtual void clear_forces() = 0;
		virtual void set_linear_velocity(const vector3 &v) = 0;
		virtual void set_angular_velocity(const vector3 &v) = 0;

	public:
		virtual void set_translation(const vector3 &v) = 0;
		virtual void set_rotation(const quat &r) = 0;

	public:
		virtual matrix get_transform_matrix() const = 0;
		virtual vector3 get_translation() const = 0;
		virtual quat get_rotation() const = 0;

	public:
		virtual void on_insert_to_physics_world(physics_world_btphysics *ptr) = 0;
		virtual void on_remove_from_physics_world(physics_world_btphysics *ptr) = 0;
	};
}
