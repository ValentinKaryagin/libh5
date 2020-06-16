#pragma once

#include "object.h"
#include "physics.h"

namespace h5
{
	class physics_component;
	class static_mesh;

	class physics_interface
		: public object
	{
	protected:
		std::vector<physics_world *> worlds;

	protected:
		std::vector<physics_component *> components;

	protected:
		physics_interface();

	public:
		virtual ~physics_interface();

	public:
		int32_t init(size_t physics_worlds_count = 1);
		void terminate();

	public:
		int32_t simulate(float dt);

	public:
		void insert_component(physics_component *ptr, size_t physics_world_index = 0);
		void remove_component(physics_component *ptr);

	public:
		int32_t init_context(static_mesh *ptr);
		void terminate_context(static_mesh *ptr);

	public:
		static physics_interface *get_instance()
		{
			static physics_interface physics_interface_instance;
			return &physics_interface_instance;
		}
	};
}
