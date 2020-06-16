#include "..\..\libh5\h5.h"

#include <thread>

#include <conio.h>

#include <Windows.h>

#define THREADS_COUNT std::thread::hardware_concurrency()

volatile bool is_shutdown_requested = false;

unsigned long __stdcall thread(void *params);

int main()
{
	std::vector<HANDLE> threads;
	for (size_t i = 0; i < THREADS_COUNT; ++i)
	{
		Sleep(1);

		threads.push_back(CreateThread(nullptr, 0, &thread, nullptr, 0, nullptr));
	}

	while (_getch() != VK_ESCAPE);

	Sleep(1000);

	is_shutdown_requested = true;

	for (auto &item : threads)
	{
		CloseHandle(item);
	}

	return 0;
}

unsigned long __stdcall thread(void *params)
{
	printf("thread\n");

	int32_t error_level = 0;

	h5::socket socket;
	error_level = socket.init(h5::socket::ip_protocol::tcp);

	printf("socket create: %lu\n", error_level);

	if (!error_level)
	{
		error_level = socket.connect(7777, u("127.0.0.1"));
		printf("socket connect: %lu\n", error_level);
	}

	if (!error_level)
	{
		size_t bytes_transferred = 0;

		while (!is_shutdown_requested)
		{
			Sleep(1);

			error_level = socket.send((const uint8_t *)"hello, iocp!", 13, &bytes_transferred);

			uint8_t recv_buffer[1024]{};

			error_level = socket.recv(recv_buffer, 1024, &bytes_transferred);
		}
	}

	error_level = socket.disconnect();

	printf("socket disconnect: %lu\n", error_level);

	return error_level;
}
