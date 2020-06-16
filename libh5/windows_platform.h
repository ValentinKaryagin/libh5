#pragma once

#include "socket_ws2.h"
#include "iocp.h"
#include "iocp_server.h"
#include "application_window_windows.h"
#include "mutex_windows.h"
#include "guid_windows.h"
#include "thread_windows.h"
#include "event_windows.h"

#ifndef LIBH5
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#endif

namespace h5
{
	typedef socket_ws2					socket;
	typedef application_window_windows	application_window;
	typedef mutex_windows				mutex;	
	typedef guid_windows				guid;
	typedef thread_windows				thread;
	typedef event_windows				event;
}
