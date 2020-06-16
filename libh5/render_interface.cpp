#include "render_interface.h"

#include "render.h"
#include "application_interface.h"
#include "view_component.h"
#include "draw_component.h"
#include "draw_instanced_component.h"
#include "meshes.h"
#include "frustum_culling.h"

namespace h5
{
	auto render_interface_instance = render_interface::get_instance();

	const float render_interface::NEAR_PLANE_DEFAULT = 0.001f;
	const float render_interface::FAR_PLANE_DEFAULT = 1024.0f;

	struct render_context
	{
		render_buffer vertex_buffer;
		render_buffer index_buffer;
	};

	render_interface::render_interface()
		: object()
	{
		DEBUG_PRINT_FUNC();
	}

	render_interface::~render_interface()
	{
		DEBUG_PRINT_FUNC();

		terminate();
	}

	int32_t render_interface::init()
	{
		DEBUG_PRINT_FUNC();

		int32_t error_level = 0;

		application_interface::get_instance()->init_event.wait();

		render_device *device = nullptr;
		if (!error_level)
		{
			device = new (std::nothrow) render_device();
			if (!device)
			{
				error_level = ERRLVL_GF_MEMORY_ALLOCATE;
			}
		}

		if (!error_level)
		{
			error_level = device->init(application_interface::get_instance()->get_window());

			if (!error_level)
			{
				devices.push_back(device);
			}
			else
			{
				delete device;
			}			
		}		

		if (!error_level)
		{
			error_level = vs_draw.load(u("vs_draw.hlsl"), "vs_draw");
			if (!error_level)
			{
				error_level = device->create_vertex_shader(&vs_draw);
			}
		}		

		if (!error_level)
		{
			error_level = vs_draw_instanced.load(u("vs_draw_instanced.hlsl"), "vs_draw_instanced");
			if (!error_level)
			{
				error_level = device->create_vertex_shader(&vs_draw_instanced);
			}
		}

		if (!error_level)
		{
			error_level = ps.load(u("ps.hlsl"), "ps");
			if (!error_level)
			{
				error_level = device->create_pixel_shader(&ps);
				if (!error_level)
				{
					device->set_pixel_shader(&ps);
				}				
			}
		}		

		if (!error_level)
		{
			device->set_render_state(render_device::state::wireframe);
		}

		return error_level;
	}

	void render_interface::terminate()
	{
		DEBUG_PRINT_FUNC();

		components.clear();

		ps.terminate();
		vs_draw_instanced.terminate();
		vs_draw.terminate();

		for (auto &item : devices)
		{
			item->terminate();
			delete item;
			item = nullptr;
		}

		devices.clear();
	}

	void render_interface::clear()
	{
		devices[0]->clear();
	}

	int32_t render_interface::draw(render_component *ptr)
	{
		return ptr->on_draw(this);
	}

	int32_t render_interface::present()
	{
		return devices[0]->present();
	}

	int32_t render_interface::draw(draw_component *ptr)
	{
		int32_t error_level = 0;

		devices[0]->set_vertex_shader(&vs_draw);
		
		h5::matrix w, v, p, vp, wvp;
		w = ptr->get_transform_matrix();
		if (view_component_ptr)
		{
			v = view_component_ptr->get_view_matrix();
		}
		else
		{
			v.look_at(h5::vector3::zero, h5::vector3::forward, h5::vector3::up);
		}
		p.perspective(deg_to_rad(view_component_ptr ? view_component_ptr->get_fov() : 60.0f), application_interface::get_instance()->get_window()->get_aspect_ratio(), render_interface::NEAR_PLANE_DEFAULT, render_interface::FAR_PLANE_DEFAULT);
		vp = v * p;

		frustum_culling frustum(vp);

		wvp = w * vp;

		h5::render_buffer_d3d11 constant_buffer;
		error_level = devices[0]->create_buffer(h5::render_buffer_d3d11::type::constant, &wvp, 1, sizeof(h5::matrix), &constant_buffer);
		if (!error_level)
		{
			devices[0]->set_vertex_shader_constant_buffer(&constant_buffer, 0);

			static_mesh *mesh_ptr = ptr->get_mesh();
			if (!mesh_ptr)
			{
				error_level = ERRLVL_GF_NULL_MESH;
			}

			if (!error_level)
			{
				if (!mesh_ptr->render_ctx->vertex_buffer.is_valid())
				{
					error_level = ERRLVL_GF_NOT_VALID_VERTICES_BUFFER;
				}
				
				if (!error_level)
				{
					devices[0]->set_vertex_buffer(&mesh_ptr->render_ctx->vertex_buffer);

					if (!mesh_ptr->render_ctx->index_buffer.is_valid())
					{
						// todo: draw non indexed
					}
					else
					{
						devices[0]->set_index_buffer(&mesh_ptr->render_ctx->index_buffer);
						devices[0]->draw_indexed(mesh_ptr->indices.size());
					}
				}								
			}
		}	

		return error_level;
	}

	int32_t render_interface::draw(draw_instanced_component *ptr)
	{
		int32_t error_level = 0;

		devices[0]->set_vertex_shader(&vs_draw_instanced);

		h5::matrix v, p, vp;
		if (view_component_ptr)
		{
			v = view_component_ptr->get_view_matrix();
		}
		else
		{
			v.look_at(h5::vector3::zero, h5::vector3::forward, h5::vector3::up);
		}
		p.perspective(deg_to_rad(view_component_ptr ? view_component_ptr->get_fov() : 60.0f), application_interface::get_instance()->get_window()->get_aspect_ratio(), render_interface::NEAR_PLANE_DEFAULT, render_interface::FAR_PLANE_DEFAULT);
		vp = v * p;

		frustum_culling frustum(vp);

		h5::render_buffer_d3d11 constant_buffer;
		error_level = devices[0]->create_buffer(h5::render_buffer_d3d11::type::constant, &vp, 1, sizeof(h5::matrix), &constant_buffer);
		if (!error_level)
		{
			devices[0]->set_vertex_shader_constant_buffer(&constant_buffer, 0);

			h5::render_buffer_d3d11 structured_buffer;
			std::vector<matrix> transform_matrices = ptr->get_transform_matrices();
			error_level = devices[0]->create_buffer(h5::render_buffer_d3d11::type::structured_buffer, transform_matrices.data(), transform_matrices.size(), sizeof(h5::matrix), &structured_buffer);
			if (!error_level)
			{
				h5::shader_res shader_resource;
				error_level = devices[0]->create_shader_res(&structured_buffer, transform_matrices.size(), &shader_resource);
				if (!error_level)
				{
					devices[0]->set_vertex_shader_res(&shader_resource, 0);

					static_mesh *mesh_ptr = ptr->get_mesh();
					if (!mesh_ptr)
					{
						error_level = ERRLVL_GF_NULL_MESH;
					}
					
					if (!error_level)
					{
						if (!mesh_ptr->render_ctx->vertex_buffer.is_valid())
						{
							error_level = ERRLVL_GF_NOT_VALID_VERTICES_BUFFER;							
						}

						if (!error_level)
						{
							devices[0]->set_vertex_buffer(&mesh_ptr->render_ctx->vertex_buffer);

							if (!mesh_ptr->render_ctx->index_buffer.is_valid())
							{
								// todo: draw non indexed
							}
							else
							{
								devices[0]->set_index_buffer(&mesh_ptr->render_ctx->index_buffer);
								devices[0]->draw_indexed_instanced(mesh_ptr->indices.size(), transform_matrices.size());
							}
						}						
					}
				}
			}
		}

		return error_level;
	}

	int32_t render_interface::draw()
	{
		int32_t error_level = 0;

		clear();
		for (auto &item : components)
		{
			error_level = draw(item);

			if (error_level)
			{
				break;
			}
		}

		if (!error_level)
		{
			error_level = present();
		}
		
		return error_level;
	}

	render_device *render_interface::get_device()
	{
		return devices[0];
	}

	void render_interface::set_view_component(view_component *ptr)
	{
		view_component_ptr = ptr;
	}

	void render_interface::remove_view_component()
	{
		view_component_ptr = nullptr;
	}

	void render_interface::insert_component(render_component *ptr)
	{
		if (!ptr->mesh_ptr)
		{
			return;
		}		

		components.push_back(ptr);
	}

	void render_interface::remove_component(render_component *ptr)
	{
		components.erase(std::remove(components.begin(), components.end(), ptr));
	}

	int32_t render_interface::init_context(static_mesh *ptr)
	{
		int32_t error_level = 0;

		ptr->render_ctx = new render_context();
		if (!ptr->render_ctx)
		{
			error_level = ERRLVL_GF_MEMORY_ALLOCATE;
		}

		if (!error_level)
		{
			error_level = devices[0]->create_buffer(
				h5::render_buffer_d3d11::type::vertex,
				ptr->vertices.data(),
				ptr->vertices.size(),
				sizeof(h5::vertex),
				&ptr->render_ctx->vertex_buffer
			);
		}

		if (!error_level)
		{
			if (ptr->indices.size())
			{
				error_level = devices[0]->create_buffer(
					h5::render_buffer_d3d11::type::index,
					ptr->indices.data(),
					ptr->indices.size(),
					sizeof(uint32_t),
					&ptr->render_ctx->index_buffer
				);
			}		
		}

		return error_level;
	}

	void render_interface::terminate_context(static_mesh *ptr)
	{
		delete ptr->render_ctx;
		ptr->render_ctx = nullptr;
	}
}
