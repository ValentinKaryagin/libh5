#pragma once

#include "windows_public.h"

#define WIN32_CRITICAL_SECTION_SIZE 40

namespace h5
{
	class mutex_windows
	{
	protected:
		char cs[WIN32_CRITICAL_SECTION_SIZE]{};

	public:
		mutex_windows();
		virtual ~mutex_windows();

	public:
		void lock();
		bool try_lock();
		void unlock();
	};
}
