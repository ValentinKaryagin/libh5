#include "iocp.h"

#include <Windows.h>

#define IOCP_QUEUE_TIMEOUT 1000

namespace h5
{
	iocp::iocp() {}

	iocp::~iocp()
	{
		terminate();
	}

	int32_t iocp::init(size_t threads_count)
	{
		handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
		if (!handle)
		{
			DEBUG_PRINT_FILELINERES(GetLastError());
			return ERRLVL_IO_CREATE_IOCP;
		}

		if (!threads_count)
		{
			SYSTEM_INFO sys_info{};
			GetSystemInfo(&sys_info);
			threads_count = (size_t)sys_info.dwNumberOfProcessors;
		}		

		for (size_t i = 0; i < threads_count; ++i)
		{
			HANDLE thread_handle = CreateThread(nullptr, 0, &iocp::thread_worker, this, 0, nullptr);
			if (!thread_handle)
			{
				DEBUG_PRINT_FILELINERES(GetLastError());
				return ERRLVL_WIN32_CREATE_THREAD;
			}

			threads.push_back(thread_handle);
		}

		is_thread_terminate_requested = false;

		return 0;
	}

	void iocp::terminate()
	{
		size_t threads_count = threads.size();

		if (threads_count)
		{
			is_thread_terminate_requested = true;

			WaitForMultipleObjects((DWORD)threads_count, threads.data(), TRUE, INFINITE);

			for (auto &item : threads)
			{
				if (!CloseHandle(item))
				{
					DEBUG_PRINT_FILELINERES(GetLastError());
					return;
				}
			}

			threads.clear();
		}

		if (handle)
		{
			if (!CloseHandle(handle))
			{
				DEBUG_PRINT_FILELINERES(GetLastError());
				return;
			}

			handle = nullptr;
		}
	}	

	int32_t iocp::associate(void *file_handle, void *key)
	{
		handle = CreateIoCompletionPort(file_handle, handle, (ULONG_PTR)key, 0);
		return !handle ? ERRLVL_IO_CREATE_IOCP : 0;
	}	

	unsigned long __stdcall iocp::thread_worker(void *params)
	{
		iocp *iocp_ptr = (iocp *)params;
		
		DWORD bytes_transferred = 0;
		void *key = nullptr;
		OVERLAPPED *overlapped = nullptr;

		while (!iocp_ptr->is_thread_terminate_requested)
		{
			bool status = GetQueuedCompletionStatus(iocp_ptr->handle, &bytes_transferred, (ULONG_PTR *)&key, &overlapped, (DWORD)IOCP_QUEUE_TIMEOUT);
			iocp_ptr->on_io(status, key, overlapped, bytes_transferred);
		}

		return 0;
	}
}
