#pragma once

#include "windows_public.h"

namespace h5
{
	class application_window_windows
	{
	protected:
		unsigned short atom_wnd		= 0;
		unsigned long long wnd		= 0;

	public:
		bool is_need_exit			= false;

	protected:
		application_window_windows *parent_wnd	= nullptr;
		std::list<application_window_windows *>	child_wnds;

	public:
		application_window_windows();
		virtual ~application_window_windows();

	public:
		int32_t init(
			uint32_t left,
			uint32_t top,
			uint32_t size_x,
			uint32_t size_y,
			const std::wstring &title = u(""),
			application_window_windows *parent = nullptr,
			bool show = true,
			bool foreground = true);

		void terminate();

	public:
		bool is_exit_requested();

	public:
		unsigned long long get_win32_handler() const;
		void get_pos(uint32_t *x, uint32_t *y) const;
		void get_size(uint32_t *x, uint32_t *y) const;
		void get_canvas_size(uint32_t *x, uint32_t *y) const;
		float get_aspect_ratio() const;
		application_window_windows *get_parent() const;
		std::list<application_window_windows *> *get_childs();

	public:
		int32_t proc();

	public:
		void set_title(const std::wstring &str);

	public:
		static long long __stdcall wnd_proc(unsigned long long hwnd, unsigned int msg, unsigned long long wparam, long long lparam);
	};	
}
