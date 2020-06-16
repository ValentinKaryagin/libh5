#include "physics_interface.h"

#include "physics_component.h"
#include "static_mesh.h"

namespace h5
{
	auto physics_interface_instance_ptr = physics_interface::get_instance();

	struct physics_context
	{

	};

	physics_interface::physics_interface()
		: object()
	{
		DEBUG_PRINT_FUNC();
	}

	physics_interface::~physics_interface()
	{
		DEBUG_PRINT_FUNC();

		terminate();
	}

	int32_t physics_interface::init(size_t physics_worlds_count)
	{
		DEBUG_PRINT_FUNC();

		int32_t error_level = 0;

		physics_world *world = new (std::nothrow) physics_world();
		if (!world)
		{
			error_level = ERRLVL_GF_MEMORY_ALLOCATE;
		}
		
		if (!error_level)
		{
			error_level = world->init();
			if (!error_level)
			{
				worlds.push_back(world);
			}
			else
			{
				delete world;
			}
		}

		if (error_level)
		{
			for (auto &item : worlds)
			{
				item->terminate();
				delete item;
				item = nullptr;
			}

			worlds.clear();
		}

		return error_level;
	}

	void physics_interface::terminate()
	{
		DEBUG_PRINT_FUNC();

		for (auto &item : components)
		{
			item->physics_world_ptr->remove_collision_object(item->_collision_object);
			item->physics_world_ptr = nullptr;
		}

		components.clear();

		for (auto &item : worlds)
		{
			item->terminate();
			delete item;
			item = nullptr;
		}

		worlds.clear();
	}

	int32_t physics_interface::simulate(float dt)
	{
		int32_t error_level = 0;		

		if (!error_level)
		{
			for (auto &item : worlds)
			{
				error_level = item->simulate(dt);
				if (error_level)
				{
					break;
				}
			}
		}		

		return error_level;
	}

	void physics_interface::insert_component(physics_component *ptr, size_t physics_world_index)
	{
		if (!ptr->cinfo.mesh_ptr)
		{
			return;
		}		

		ptr->cinfo.mesh_ptr->inc_ref();

		ptr->_collision_object = new rigid_body(
			rigid_body::construct_info(
				ptr->cinfo.shape_type,
				ptr->cinfo.collision_type,
				ptr->cinfo.mesh_ptr,
				ptr->cinfo.mass
			)
		);

		components.push_back(ptr);
		worlds[physics_world_index]->insert_collision_object(ptr->_collision_object);
		ptr->physics_world_ptr = worlds[physics_world_index];
	}

	void physics_interface::remove_component(physics_component *ptr)
	{
		ptr->physics_world_ptr->remove_collision_object(ptr->_collision_object);
		ptr->physics_world_ptr = nullptr;
		components.erase(std::remove(components.begin(), components.end(), ptr));
		
		ptr->cinfo.mesh_ptr->dec_ref();
	}

	int32_t physics_interface::init_context(static_mesh *ptr)
	{
		int32_t error_level = 0;

		ptr->physics_ctx = new physics_context();

		return error_level;
	}

	void physics_interface::terminate_context(static_mesh *ptr)
	{
		delete ptr->physics_ctx;
		ptr->physics_ctx = nullptr;
	}
}
