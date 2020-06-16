#include "physics_world_btphysics.h"

#include "btphysics_private.h"

#include "collision_object_btphysics.h"
#include "rigid_body_btphysics.h"

namespace h5
{
	class physics_configurator_btphysics
	{
	public:
		btDefaultCollisionConfiguration		bt_configuration;
		btCollisionDispatcher				bt_dispatcher;
		btDbvtBroadphase					bt_pair_cache;
		btSequentialImpulseConstraintSolver	bt_solver;

	protected:
		physics_configurator_btphysics()
			: bt_dispatcher(btCollisionDispatcher(&bt_configuration))
		{}

	public:
		virtual ~physics_configurator_btphysics() {}

	public:
		static physics_configurator_btphysics *get_instance()
		{
			static physics_configurator_btphysics physics_configurator_btphysics_instance;
			return &physics_configurator_btphysics_instance;
		}
	};
}

namespace h5
{
	physics_world_btphysics::physics_world_btphysics() {}

	physics_world_btphysics::~physics_world_btphysics()
	{
		terminate();
	}

	int32_t physics_world_btphysics::init()
	{
		int32_t error_level = 0;

		physics_configurator_btphysics *physics_configurator = physics_configurator_btphysics::get_instance();

		bt_world = new btDiscreteDynamicsWorld(
			&physics_configurator->bt_dispatcher,
			&physics_configurator->bt_pair_cache,
			&physics_configurator->bt_solver,
			&physics_configurator->bt_configuration);
		if (!bt_world)
		{
			error_level = ERRLVL_BTPHYS_MEMORY_ALLOCATE;
		}

		bt_world->setGravity(btVector3(0.0f, 0.0f, -9.8f));

		if (!error_level)
		{
			bt_world->stepSimulation(1.0f / 60.0f);

			/*if (!bt_world->stepSimulation(1.0f / 60.0f))
			{
				error_level = ERRLVL_BTPHYS_NOT_SIMULATED;
			}*/		
		}

		return error_level;
	}

	void physics_world_btphysics::terminate()
	{
		BULLET_PHYSICS_SAFE_RELEASE(bt_world);
	}	

	int32_t physics_world_btphysics::simulate(float dt)
	{
		int32_t error_level = 0;

		bt_world->stepSimulation(dt);

		/*if (!bt_world->stepSimulation(dt))
		{
			error_level = ERRLVL_BTPHYS_NOT_SIMULATED;
		}*/

		return error_level;
	}

	void physics_world_btphysics::set_gravity(const vector3 &direction, float value)
	{
		bt_world->setGravity(btVector3(direction.x * value, direction.y * value, direction.z * value));
	}

	void physics_world_btphysics::set_gravity(const vector3 &vector)
	{
		bt_world->setGravity(btVector3(vector.x, vector.y, vector.z));
	}

	void physics_world_btphysics::insert_collision_object(collision_object_btphysics *ptr)
	{
		ptr->on_insert_to_physics_world(this);
	}

	void physics_world_btphysics::remove_collision_object(collision_object_btphysics *ptr)
	{
		ptr->on_remove_from_physics_world(this);
	}

	void physics_world_btphysics::insert_rigid_body(rigid_body_btphysics *ptr)
	{
		bt_world->addRigidBody(ptr->bt_rigid_body);
	}

	void physics_world_btphysics::remove_rigid_body(rigid_body_btphysics *ptr)
	{
		bt_world->removeRigidBody(ptr->bt_rigid_body);
	}
}
