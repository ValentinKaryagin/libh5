#pragma once

#include "platform.h"

namespace h5
{
	class lock_guard
	{
	protected:
		mutex *locked_mutex = nullptr;;

	public:
		__forceinline lock_guard(mutex *mutex_to_lock)
		{
			locked_mutex = mutex_to_lock;
			locked_mutex->lock();
		}

		__forceinline ~lock_guard()
		{
			locked_mutex->unlock();
		}
	};
}
