#pragma once

#include "commons.h"

namespace h5
{
	template<typename T>
	class allocator
	{
	public:
		static const size_t OBJECTS_COUNT = 1024;

	protected:
		T *memory = nullptr;
		std::list<T *> free_ptrs;

	public:
		__forceinline allocator()
			: memory((T *)malloc(allocator::OBJECTS_COUNT * sizeof(T)))
		{
			//DEBUG_PRINT_FUNC();

			if (memory)
			{
				for (size_t i = 0; i < allocator::OBJECTS_COUNT; ++i)
				{
					free_ptrs.push_back(&memory[i]);
				}
			}			
		}

		__forceinline virtual ~allocator()
		{
			//DEBUG_PRINT_FUNC();

			free(memory);
		}

	public:
		template<typename...Args>
		__forceinline T *allocate(Args...args)
		{
			T *result = nullptr;

			if (free_ptrs.empty())
			{
				return result;
			}

			result = new (free_ptrs.front()) T(args...);
			free_ptrs.pop_front();

			return result;
		}

		__forceinline void deallocate(T **ptr)
		{
			(*ptr)->~T();
			free_ptrs.push_back(*ptr);
			(*ptr)->allocator_ptr = nullptr;
			*ptr = nullptr;
		}

		__forceinline bool is_empty()
		{
			return free_ptrs.size() == allocator::OBJECTS_COUNT;
		}

		__forceinline bool is_full()
		{
			return free_ptrs.empty();
		}
	};
}
