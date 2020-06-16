#pragma once

#include "btphysics_public.h"
#include "math.h"

namespace h5
{
	class collision_object_btphysics;
	class rigid_body_btphysics;

	class physics_world_btphysics
	{
	public:
		friend class rigid_body_btphysics;

	protected:
		btDiscreteDynamicsWorld *bt_world = nullptr;

	public:
		physics_world_btphysics();
		virtual ~physics_world_btphysics();

	public:
		int32_t init();
		void terminate();

	public:
		int32_t simulate(float dt);

	public:
		void set_gravity(const vector3 &direction, float value);
		void set_gravity(const vector3 &vector);

	public:
		void insert_collision_object(collision_object_btphysics *ptr);
		void remove_collision_object(collision_object_btphysics *ptr);

	public:
		void insert_rigid_body(rigid_body_btphysics *ptr);
		void remove_rigid_body(rigid_body_btphysics *ptr);
	};
}
