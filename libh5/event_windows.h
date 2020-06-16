#pragma once

#include "windows_public.h"

namespace h5
{
	class event_windows
	{
		void *handle = nullptr;

	public:
		event_windows();
		virtual ~event_windows();

	public:
		void set();
		void wait();
	};	
}
