#pragma once

#include "windows_public.h"

struct _OVERLAPPED;

namespace h5
{
	class iocp
	{
	protected:
		volatile bool is_thread_terminate_requested = false;

	protected:
		void *handle = nullptr;
		std::vector<void *> threads;

	public:
		iocp();
		virtual ~iocp();

	public:
		int32_t init(size_t threads_count = 0);
		void terminate();

	protected:
		int32_t associate(void *file_handle, void *key);		

	public:
		virtual void on_io(bool status, void *key, _OVERLAPPED *overlapped, size_t bytes_transferred) {};

	public:
		static unsigned long __stdcall thread_worker(void *params);
	};
}
