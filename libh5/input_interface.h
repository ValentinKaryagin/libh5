#pragma once

#include "object.h"
#include "input.h"

namespace h5
{
	class application_interface;

	class input_interface
		: public object
	{
	protected:
		mutex devices_mtx;
		std::vector<input_device *> devices;

	protected:
		mutex new_connected_devices_mtx;
		std::queue<input_device *> new_connected_devices;

	protected:
		mutex is_terminate_requested_mtx;
		bool _is_terminate_requested = false;

	protected:
		thread input_devices_check_thread;
		thread input_devices_read_thread;

	protected:
		input_interface();

	public:
		virtual ~input_interface();

	public:
		int32_t init();
		void terminate();

	public:
		int32_t	read();

	public:
		void request_terminate();
		bool is_terminate_requested();

	public:
		static int32_t check_devices(guid *id_ptr, input_device::type type, const wchar_t *device_name);

	public:
		static void input_devices_check_thread_worker(void *params);
		static void input_devices_read_thread_worker(void *params);

	public:
		int32_t init_threaded();
		void terminate_threaded();

	public:
		static input_interface *get_instance()
		{
			static input_interface input_interface_instance;
			return &input_interface_instance;
		}
	};
}
