#pragma once

#include "object.h"
#include "render.h"

namespace h5
{
	class application_interface;
	class view_component;
	class render_component;
	class draw_component;
	class draw_instanced_component;	
	class static_mesh;

	class render_interface
		: public object
	{
	public:
		static const float NEAR_PLANE_DEFAULT;
		static const float FAR_PLANE_DEFAULT;

	protected:
		std::vector<render_device *> devices;

	protected:
		vertex_shader	vs_draw;
		vertex_shader	vs_draw_instanced;
		pixel_shader	ps;

	protected:
		view_component *view_component_ptr = nullptr;
		std::vector<render_component *> components;

	protected:
		render_interface();

	public:
		virtual ~render_interface();

	public:
		int32_t init();
		void terminate();

	public:
		void clear();
		int32_t draw(render_component *ptr);
		int32_t present();

	public:
		int32_t draw(draw_component *ptr);
		int32_t draw(draw_instanced_component *ptr);

	public:
		int32_t draw();

	public:
		render_device *get_device();

	public:
		void set_view_component(view_component *ptr);
		void remove_view_component();
		void insert_component(render_component *ptr);
		void remove_component(render_component *ptr);

	public:
		int32_t init_context(static_mesh *ptr);
		void terminate_context(static_mesh *ptr);

	public:
		static render_interface *get_instance()
		{
			static render_interface render_interface_instance;
			return &render_interface_instance;
		}
	};
}
