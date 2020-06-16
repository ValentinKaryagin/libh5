#include "mutex_windows.h"

#include <Windows.h>

namespace h5
{
	mutex_windows::mutex_windows()
	{
		InitializeCriticalSection((CRITICAL_SECTION *)cs);
	}

	mutex_windows::~mutex_windows()
	{
		DeleteCriticalSection((CRITICAL_SECTION *)cs);
	}

	void mutex_windows::lock()
	{
		EnterCriticalSection((CRITICAL_SECTION *)cs);
	}

	bool mutex_windows::try_lock()
	{
		return static_cast<bool>(TryEnterCriticalSection((CRITICAL_SECTION *)cs));
	}

	void mutex_windows::unlock()
	{
		LeaveCriticalSection((CRITICAL_SECTION *)cs);
	}
}
