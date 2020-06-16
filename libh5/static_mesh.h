#pragma once

#include "object.h"

namespace h5
{
	struct physics_context;
	struct render_context;

	class static_mesh
		: public object
	{
	public:
		friend class physics_interface;
		friend class render_interface;
		
	public:
		friend class physics_component;
		friend class render_component;
		
	protected:
		std::wstring _path;

	protected:
		size_t refs = 0;

	protected:
		size_t render_refs = 0;
		size_t physics_refs = 0;

	protected:
		std::vector<vertex>		vertices;
		std::vector<uint32_t>	indices;

	protected:
		physics_context *physics_ctx = nullptr;
		render_context *render_ctx = nullptr;		

	public:
		static_mesh();
		static_mesh(const std::wstring &path);
		virtual ~static_mesh();

	public:
		virtual void set_path(const std::wstring &path);

	public:
		virtual int32_t init();
		void terminate();

	public:
		int32_t read_native();
		int32_t read_wavefront();

	public:
		int32_t write_native();

	protected:
		size_t inc_ref();
		size_t dec_ref();
	};
}
