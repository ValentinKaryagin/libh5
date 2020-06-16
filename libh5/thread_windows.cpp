#include "thread_windows.h"

#include <Windows.h>

namespace h5
{
	thread_windows::thread_windows() {}

	thread_windows::~thread_windows()
	{
		terminate();
	}

	void thread_windows::exit(int32_t exit_code)
	{
		ExitThread((DWORD)exit_code);
	}

	void thread_windows::sleep(uint32_t ms)
	{
		Sleep((DWORD)ms);
	}

	int32_t thread_windows::init(thread_func *func_ptr, void *params)
	{
		int32_t error_level = 0;

		handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)func_ptr, params, 0, (DWORD *)&thread_id);
		if (!handle)
		{
			error_level = ERRLVL_WIN32_CREATE_THREAD;
		}

		return error_level;
	}

	void thread_windows::terminate()
	{		
		if (handle)
		{
			WaitForSingleObject(handle, INFINITE);
			CloseHandle(handle);
			handle = nullptr;
			thread_id = 0;
		}
	}

	void thread_windows::force_terminate()
	{
		if (handle)
		{
			TerminateThread(handle, 0);
			CloseHandle(handle);
			handle = nullptr;
			thread_id = 0;
		}		
	}

	void thread_windows::wait()
	{
		WaitForSingleObject(handle, INFINITE);
	}
}
