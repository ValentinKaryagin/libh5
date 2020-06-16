#include "application_interface.h"

namespace h5
{
	auto application_interface_instance_ptr = application_interface::get_instance();

	application_interface::application_interface()
		: object()
	{
		DEBUG_PRINT_FUNC();
	}

	application_interface::~application_interface()
	{
		DEBUG_PRINT_FUNC();
		terminate();
	}

	int32_t application_interface::init()
	{
		DEBUG_PRINT_FUNC();

		int32_t error_level = 0;

		application_window *window = new (std::nothrow) application_window();
		if (!window)
		{
			error_level = ERRLVL_GF_MEMORY_ALLOCATE;
		}

		if (!error_level)
		{
			error_level = window->init(128, 128, 1280, 720, u("h5"));

			if (!error_level)
			{
				windows.push_back(window);
			}
			else
			{
				delete window;
			}
		}		

		if (!error_level)
		{
			_is_terminate_requested = false;
		}

		if (!error_level)
		{
			init_event.set();
		}

		return error_level;
	}

	void application_interface::terminate()
	{
		DEBUG_PRINT_FUNC();

		request_terminate();

		for (auto &item : windows)
		{
			item->terminate();
			delete item;
			item = nullptr;
		}

		windows.clear();
	}	

	int32_t application_interface::proc()
	{
		return windows[0]->proc();
	}

	application_window *application_interface::get_window()
	{
		return windows[0];
	}

	void application_interface::request_terminate()
	{
		is_terminate_requested_mtx.lock();
		_is_terminate_requested = true;
		is_terminate_requested_mtx.unlock();
	}

	bool application_interface::is_terminate_requested()
	{
		bool result = false;
		is_terminate_requested_mtx.lock();
		result = _is_terminate_requested;
		is_terminate_requested_mtx.unlock();
		return result;
	}

	void application_interface::application_proc_thread_worker(void *params)
	{
		int32_t error_level = 0;
		
		error_level = application_interface::get_instance()->init();
				
		while (!error_level && !application_interface::get_instance()->is_terminate_requested())
		{
			error_level = application_interface::get_instance()->proc();
		}

		application_interface::get_instance()->terminate();

		DEBUG_PRINTF("application thread result: %d", error_level);

		thread::exit(error_level);
	}

	int32_t application_interface::init_threaded()
	{
		int32_t error_level = 0;		

		error_level = application_proc_thread.init(&application_interface::application_proc_thread_worker);

		return error_level;
	}

	void application_interface::terminate_threaded()
	{
		request_terminate();

		application_proc_thread.terminate();
	}
}
