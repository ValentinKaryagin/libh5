#pragma once

#include "object.h"
#include "platform.h"

namespace h5
{
	class application_interface
		: public object
	{
	public:
		event init_event;

	protected:
		std::vector<application_window *> windows;

	protected:
		mutex is_terminate_requested_mtx;
		bool _is_terminate_requested = false;

	protected:
		thread application_proc_thread;

	protected:
		application_interface();

	public:
		virtual ~application_interface();

	public:
		int32_t init();
		void terminate();
	public:
		int32_t proc();

	public:
		application_window *get_window();

	public:
		void request_terminate();
		bool is_terminate_requested();

	public:
		static void application_proc_thread_worker(void *params);

	public:
		int32_t init_threaded();
		void terminate_threaded();

	public:
		static application_interface *get_instance()
		{
			static application_interface application_interface_instance;
			return &application_interface_instance;
		}
	};
}
