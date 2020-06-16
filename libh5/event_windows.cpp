#include "event_windows.h"

#include <Windows.h>

namespace h5
{
	event_windows::event_windows()
	{
		handle = CreateEventW(NULL, TRUE, FALSE, NULL);
	}

	event_windows::~event_windows()
	{
		CloseHandle(handle);
	}

	void event_windows::set()
	{
		SetEvent(handle);
	}

	void event_windows::wait()
	{
		WaitForSingleObject(handle, INFINITE);
	}
}
