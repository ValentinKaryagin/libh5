#include "input_interface.h"

#include "application_interface.h"

namespace h5
{
	auto input_interface_instance_ptr = input_interface::get_instance();

	input_interface::input_interface()
		: object()
	{
		DEBUG_PRINT_FUNC();
	}

	input_interface::~input_interface()
	{
		DEBUG_PRINT_FUNC();

		terminate();
	}

	int32_t input_interface::init()
	{
		DEBUG_PRINT_FUNC();

		int32_t error_level = 0;

		error_level = input_device::enum_devices(&input_interface::check_devices);

		return error_level;
	}

	void input_interface::terminate()
	{
		DEBUG_PRINT_FUNC();

		request_terminate();

		devices_mtx.lock();
		for (auto &item : devices)
		{
			item->terminate();
			delete item;
		}

		devices.clear();
		devices_mtx.unlock();
	}

	int32_t input_interface::read()
	{
		int32_t error_level = 0;

		new_connected_devices_mtx.lock();
		while (!new_connected_devices.empty())
		{
			auto new_device = new_connected_devices.front();

			devices_mtx.lock();
			devices.push_back(new_device);
			devices_mtx.unlock();

			new_connected_devices.pop();
		}
		new_connected_devices_mtx.unlock();

		devices_mtx.lock();
		auto devices_copy = devices;
		for (auto &item : devices_copy)
		{
			error_level = item->read();

			if (error_level)
			{
				if (error_level == ERRLVL_DINPUT_DEVICE_DISCONNECTED)
				{
					DEBUG_WPRINTF("[%s:%s] input device disconnected", item->get_device_id()->to_wstring().c_str(), item->get_device_name().c_str());

					item->terminate();						
					devices.erase(std::remove(devices.begin(), devices.end(), item));					
					delete item;
					error_level = 0;
					continue;
				}
			}
		}
		devices_mtx.unlock();

		return error_level;
	}

	void input_interface::request_terminate()
	{
		is_terminate_requested_mtx.lock();
		_is_terminate_requested = true;
		is_terminate_requested_mtx.unlock();
	}

	bool input_interface::is_terminate_requested()
	{
		bool result = false;
		is_terminate_requested_mtx.lock();
		result = _is_terminate_requested;
		is_terminate_requested_mtx.unlock();
		return result;
	}

	int32_t input_interface::check_devices(guid *id_ptr, input_device::type type, const wchar_t *device_name)
	{
		int32_t error_level = 0;
		bool is_device_registered = false;

		if (input_interface::get_instance()->is_terminate_requested())
		{
			return error_level;
		}

		input_interface::get_instance()->devices_mtx.lock();
		for (auto &item : input_interface::get_instance()->devices)
		{
			if (*item->get_device_id() == *id_ptr)
			{
				is_device_registered = true;
			}
		}
		input_interface::get_instance()->devices_mtx.unlock();

		if (!is_device_registered)
		{
			if (type == input_device::type::mouse)
			{
				mouse_device *mouse = new (std::nothrow) mouse_device();
				if (!mouse)
				{
					error_level = ERRLVL_GF_MEMORY_ALLOCATE;
				}

				if (!error_level)
				{
					error_level = mouse->init(application_interface::get_instance()->get_window(), *id_ptr, device_name);
				}

				if (!error_level)
				{
					input_interface::get_instance()->new_connected_devices_mtx.lock();
					input_interface::get_instance()->new_connected_devices.push(mouse);
					input_interface::get_instance()->new_connected_devices_mtx.unlock();

					DEBUG_WPRINTF(u("[%s:%s] input device connected"), id_ptr->to_wstring().c_str(), device_name);
				}
				else
				{
					delete mouse;
				}
			}
			else if (type == input_device::type::keyboard)
			{
				keyboard_device *keyboard = new (std::nothrow) keyboard_device();
				if (!keyboard)
				{
					error_level = ERRLVL_GF_MEMORY_ALLOCATE;
				}

				if (!error_level)
				{
					error_level = keyboard->init(application_interface::get_instance()->get_window(), *id_ptr, device_name);
				}

				if (!error_level)
				{
					input_interface::get_instance()->new_connected_devices_mtx.lock();
					input_interface::get_instance()->new_connected_devices.push(keyboard);
					input_interface::get_instance()->new_connected_devices_mtx.unlock();

					DEBUG_WPRINTF(u("[%s:%s] input device connected"), id_ptr->to_wstring().c_str(), device_name);
				}
				else
				{
					delete keyboard;
				}
			}
			else if (type == input_device::type::game)
			{
				game_device *game = new (std::nothrow) game_device();
				if (!game)
				{
					error_level = ERRLVL_GF_MEMORY_ALLOCATE;
				}

				if (!error_level)
				{
					error_level = game->init(application_interface::get_instance()->get_window(), *id_ptr, device_name);
				}

				if (!error_level)
				{
					input_interface::get_instance()->new_connected_devices_mtx.lock();
					input_interface::get_instance()->new_connected_devices.push(game);
					input_interface::get_instance()->new_connected_devices_mtx.unlock();

					DEBUG_WPRINTF(u("[%s:%s] input device connected"), id_ptr->to_wstring().c_str(), device_name);
				}
				else
				{
					delete game;
				}
			}
		}		

		return error_level;
	}

	void input_interface::input_devices_check_thread_worker(void *params)
	{
		int32_t error_level = 0;

		error_level = input_interface::get_instance()->init();

		while (!error_level && !input_interface::get_instance()->is_terminate_requested())
		{
			error_level = input_device::enum_devices(&input_interface::check_devices);
		}

		input_interface::get_instance()->terminate();

		DEBUG_PRINTF("input devices check thread result: %d", error_level);

		thread::exit(error_level);
	}

	void input_interface::input_devices_read_thread_worker(void *params)
	{
		int32_t error_level = 0;

		while (!error_level && !input_interface::get_instance()->is_terminate_requested())
		{
			error_level = input_interface::get_instance()->read();
		}		

		DEBUG_PRINTF("input devices proc thread result: %d", error_level);

		thread::exit(error_level);
	}

	int32_t input_interface::init_threaded()
	{
		int32_t error_level = 0;

		application_interface::get_instance()->init_event.wait();

		error_level = input_devices_check_thread.init(&input_interface::input_devices_check_thread_worker);

		if (!error_level)
		{
			error_level = input_devices_read_thread.init(&input_interface::input_devices_read_thread_worker);
		}

		return error_level;
	}

	void input_interface::terminate_threaded()
	{
		request_terminate();

		input_devices_read_thread.terminate();
		input_devices_check_thread.terminate();
	}
}
