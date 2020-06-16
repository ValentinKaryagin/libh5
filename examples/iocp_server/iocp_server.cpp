#include "server.h"

#include <conio.h>

#include <Windows.h>

int main()
{
	// result
	int32_t error_level = 0;

	// iocp server
	server srv;
	error_level = srv.init(7777, "127.0.0.1");

	// loop
	if (!error_level)
	{
		while (_getch() != VK_ESCAPE);
	}

	return error_level;
}
