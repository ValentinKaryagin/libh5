#pragma once

#include "allocator.h"

#define CREATE_ALLOCATOR_POOL(type) auto allocator_pool_ptr = h5::allocator_pool<type>::get_instance()

namespace h5
{
	template<typename T>
	class allocator_pool
	{
	protected:
		std::list<allocator<T> *> allocators;

	protected:
		__forceinline allocator_pool()
		{
			DEBUG_PRINT_FUNC();

			allocators.push_back(new allocator<T>());
		}

	public:
		__forceinline virtual ~allocator_pool()
		{
			DEBUG_PRINT_FUNC();

			if (!allocators.empty())
			{
				for (auto &item : allocators)
				{
					delete item;
				}
			}
		}

	public:
		template<typename...Args>
		T *allocate(Args...args)
		{
			T *result = nullptr;
			allocator<T> *allocator_ptr = nullptr;

			for (auto &item : allocators)
			{
				if (!item->is_full())
				{
					allocator_ptr = item;
					break;
				}
			}

			if (!allocator_ptr)
			{
				allocator_ptr = new allocator<T>();
				allocators.push_back(allocator_ptr);
			}

			result = allocator_ptr->allocate(args...);
			if (result)
			{
				result->allocator_ptr = reinterpret_cast<void *>(allocator_ptr);
			}

			return result;
		}

		void deallocate(T **ptr)
		{
			if ((*ptr)->allocator_ptr)
			{
				reinterpret_cast<allocator<T> *>((*ptr)->allocator_ptr)->deallocate(ptr);
			}						
		}

	public:
		static allocator_pool *get_instance()
		{
			static allocator_pool<T> allocator_pool_instance;
			return &allocator_pool_instance;
		}
	};
}
