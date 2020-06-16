#pragma once

#include "windows_public.h"

namespace h5
{
	typedef void(thread_func)(void *);

	class thread_windows
	{
	protected:
		uint32_t thread_id = 0;
		void *handle = nullptr;

	public:
		thread_windows();
		virtual ~thread_windows();

	public:
		static void exit(int32_t exit_code = 0);
		static void sleep(uint32_t ms);

	public:
		int32_t init(thread_func *func_ptr, void *params = nullptr);
		void terminate();

	public:
		void force_terminate();
		void wait();
	};	
}
