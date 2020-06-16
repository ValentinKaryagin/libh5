#pragma once

#include "object.h"

namespace h5
{
	class transform_component;
	class physics_component;
	class view_component;
	class draw_component;	

	class scene
		: public object
	{
	protected:
		std::vector<transform_component *> components;

	public:
		scene();
		virtual ~scene();

	public:
		virtual int32_t init();
		virtual void terminate();

	public:
		template<class T, typename...Args>
		__forceinline T *create(Args...args)
		{
			return object::create<T>(args...);
		}

		template<class T>
		__forceinline void destroy(T **object)
		{
			object::destroy<T>(object);
		}

	public:
		void insert_component(transform_component *ptr);
		void remove_component(transform_component *ptr);

	public:
		void insert(transform_component *ptr);
		void remove(transform_component *ptr);

	public:
		void set_view_component(view_component *ptr);
		void remove_view_component();

	public:
		std::vector<transform_component *> *get_components();
	};
}
