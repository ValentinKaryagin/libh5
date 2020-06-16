#pragma once

#include "platform.h"
#include "commons.h"
#include "math.h"
#include "allocator_pool.h"
#include "misc.h"

namespace h5
{
	class game_engine;

	class object
	{
	public:
		template<typename T> friend class allocator;
		template<typename T> friend class allocator_pool;		

	protected:
		void *allocator_ptr = nullptr;

	protected:
		guid id;

	public:
		object();
		virtual ~object();

	public:
		const guid *get_guid() const;

	public:
		template<typename T, typename...Args>
		T *create(Args...args)
		{
			return allocator_pool<T>::get_instance()->allocate(args...);
		}

		template<typename T>
		void destroy(T **object)
		{
			allocator_pool<T>::get_instance()->deallocate(object);
		}
	};
}
