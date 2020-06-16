#pragma once

#include "transform_component.h"

namespace h5
{
	class render_interface;
	class static_mesh;
	class scene;

	class render_component
		: public transform_component
	{
	public:
		friend class render_interface;

	public:
		struct construct_info
		{
			static_mesh *mesh_ptr = nullptr;

			construct_info() {}

			construct_info(static_mesh *mesh_ptr)
				: mesh_ptr(mesh_ptr)
			{}
		};

	protected:
		static_mesh *mesh_ptr = nullptr;

	public:
		render_component();
		render_component(const construct_info &info);
		virtual ~render_component();

	public:
		static_mesh *get_mesh();

	public:
		virtual int32_t on_draw(render_interface *ptr) = 0;

	public:
		virtual void on_insert_to_scene(scene *ptr) override;
		virtual void on_remove_from_scene(scene *ptr) override;
	};
}
