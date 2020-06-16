#include "scene.h"

#include "transform_component.h"
#include "physics_component.h"
#include "audio_component.h"
#include "view_component.h"
#include "draw_component.h"

#include "game_engine.h"

namespace h5
{
	CREATE_ALLOCATOR_POOL(scene);

	scene::scene()
		: object()
	{
		DEBUG_PRINT_FUNC();
	}

	scene::~scene()
	{
		DEBUG_PRINT_FUNC();

		terminate();
	}

	int32_t scene::init()
	{
		DEBUG_PRINT_FUNC();

		int32_t error_level = 0;

		
		
		return error_level;
	}

	void scene::terminate()
	{
		DEBUG_PRINT_FUNC();

		auto components_copy = components;
		for (auto &item : components_copy)
		{
			remove_component(item);
		}
	}

	void scene::insert_component(transform_component *ptr)
	{
		ptr->on_insert_to_scene(this);
	}

	void scene::remove_component(transform_component *ptr)
	{
		ptr->on_remove_from_scene(this);
	}

	void scene::insert(transform_component *ptr)
	{
		components.push_back(ptr);
	}

	void scene::remove(transform_component *ptr)
	{
		components.erase(std::remove(components.begin(), components.end(), ptr));
	}

	void scene::set_view_component(view_component *ptr)
	{
		render_interface::get_instance()->set_view_component(ptr);
	}

	void scene::remove_view_component()
	{
		render_interface::get_instance()->remove_view_component();
	}

	std::vector<transform_component *> *scene::get_components()
	{
		return &components;
	}
}
