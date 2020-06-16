#include "application_window_windows.h"

#include <Windows.h>

namespace h5
{
	application_window_windows::application_window_windows() {}

	application_window_windows::~application_window_windows()
	{
		terminate();
	}

	int32_t application_window_windows::init(
		uint32_t left,
		uint32_t top,
		uint32_t size_x,
		uint32_t size_y,
		const std::wstring &title,
		application_window_windows *parent,
		bool show,
		bool foreground)
	{
		HINSTANCE instance = (HINSTANCE)GetModuleHandleW(nullptr);
		if (!instance)
		{
			return ERRLVL_WIN32_GET_MODULE_HANDLE;
		}

		WNDCLASSEXW wcex{};
		wcex.cbSize = sizeof(WNDCLASSEXW);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = (WNDPROC)&application_window_windows::wnd_proc;
		wcex.hIcon = LoadIcon(instance, IDI_WINLOGO);
		wcex.hIconSm = LoadIcon(instance, IDI_WINLOGO);
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hInstance = instance;
		wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		wcex.lpszClassName = title.c_str();

		atom_wnd = RegisterClassExW(&wcex);
		if (!atom_wnd)
		{
			return ERRLVL_WIN32_REGISTER_WINDOW_CLASS;
		}

		//DWORD window_style = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME;
		DWORD window_style = WS_VISIBLE | WS_POPUP;

		RECT window_rect{};
		window_rect.left = left;
		window_rect.top = top;
		window_rect.right = window_rect.left + size_x;
		window_rect.bottom = window_rect.top + size_y;
		/*if (!AdjustWindowRectEx(&window_rect, window_style, 0, 0))
		{
			return ERRLVL_WIN32_ADJUST_WINDOW_RECT;
		}*/

		wnd = (unsigned long long)CreateWindowExW(
			0,
			(wchar_t *)((ULONG_PTR)((WORD)(atom_wnd))),
			title.c_str(),
			window_style,
			window_rect.left,
			window_rect.top,
			window_rect.right - window_rect.left,
			window_rect.bottom - window_rect.top,
			nullptr,
			nullptr,
			instance,
			parent ? (HWND)parent->wnd : nullptr);

		if (!wnd)
		{
			return ERRLVL_WIN32_CREATE_WINDOW;
		}

		parent_wnd = parent;

		if (!ShowWindow((HWND)wnd, show ? SW_SHOW : SW_HIDE))
		{
			return ERRLVL_WIN32_SHOW_WINDOW;
		}

		if (!UpdateWindow((HWND)wnd))
		{
			return ERRLVL_WIN32_UPDATE_WINDOW;
		}

		if (foreground)
		{
			if (!SetForegroundWindow((HWND)wnd))
			{
				return ERRLVL_WIN32_SET_FOREGROUND_WINDOW;
			}
		}

		if (!SetPropW((HWND)wnd, u("h5"), reinterpret_cast<HANDLE>(this)))
		{
			return ERRLVL_WIN32_SET_PROP_WINDOW;
		}

		return 0;
	}

	void application_window_windows::terminate()
	{
		if (wnd)
		{
			if (!DestroyWindow((HWND)wnd))
			{
				DEBUG_PRINT_FILELINE();
				//ERRLVL_WIN32_DESTROY_WINDOW;
			}

			wnd = 0;
		}	

		if (atom_wnd)
		{
			if (!UnregisterClassW((wchar_t *)((ULONG_PTR)((WORD)(atom_wnd))), (HINSTANCE)GetModuleHandleW(nullptr)))
			{
				DEBUG_PRINT_FILELINE();
				//ERRLVL_WIN32_UNREGISTER_WINDOW_CLASS;
			}

			atom_wnd = 0;
		}		

		if (parent_wnd)
		{
			parent_wnd->child_wnds.remove(this);
			parent_wnd = nullptr;
		}

		if (!child_wnds.empty())
		{
			for (auto &item : child_wnds)
			{
				item->parent_wnd = nullptr;
				item->terminate();
			}
			child_wnds.clear();
		}
	}

	bool application_window_windows::is_exit_requested()
	{
		return is_need_exit;
	}

	unsigned long long application_window_windows::get_win32_handler() const
	{
		return wnd;
	}

	void application_window_windows::get_pos(uint32_t *x, uint32_t *y) const
	{
		RECT rect{};
		GetWindowRect((HWND)wnd, &rect);
		*x = rect.left;
		*y = rect.top;
	}

	void application_window_windows::get_size(uint32_t *x, uint32_t *y) const
	{
		RECT rect{};
		GetWindowRect((HWND)wnd, &rect);
		*x = rect.right - rect.left;
		*y = rect.bottom - rect.top;
	}

	void application_window_windows::get_canvas_size(uint32_t *x, uint32_t *y) const
	{
		RECT rect{};
		GetClientRect((HWND)wnd, &rect);
		*x = rect.right;
		*y = rect.bottom;
	}

	float application_window_windows::get_aspect_ratio() const
	{
		RECT rect{};
		GetClientRect((HWND)wnd, &rect);
		return (float)rect.right / (float)rect.bottom;
	}

	application_window_windows *application_window_windows::get_parent() const
	{
		return parent_wnd;
	}

	std::list<application_window_windows *> *application_window_windows::get_childs()
	{
		return &child_wnds;
	}

	int32_t application_window_windows::proc()
	{
		MSG msg{};
		while (PeekMessageW(&msg, (HWND)wnd, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				is_need_exit = true;
			}
			else
			{
				if (!TranslateMessage(&msg))
				{
					// info: not an error, simply because the message may not be posted
					//DEBUG_PRINT_FILELINE();
					//return ERRLVL_WIN32_TRANSLATE_MESSAGE;
				}

				if (FAILED(DispatchMessageW(&msg)))
				{
					return ERRLVL_WIN32_DISPATCH_MESSAGE;
				}
			}
		}

		return 0;
	}

	void application_window_windows::set_title(const std::wstring &str)
	{
		SetWindowTextW((HWND)wnd, str.c_str());
	}

	long long __stdcall application_window_windows::wnd_proc(unsigned long long hwnd, unsigned int msg, unsigned long long wparam, long long lparam)
	{
		application_window_windows *window = reinterpret_cast<application_window_windows *>(GetPropW((HWND)hwnd, u("h5")));

		switch (msg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);
				if (window)
				{
					window->is_need_exit = true;
				}
				break;
			case WM_SIZE:
				if (window)
				{
					uint32_t x = 0, y = 0;
					window->get_canvas_size(&x, &y);
				}
				break;
			default:
				break;
		}

		return DefWindowProcW((HWND)hwnd, msg, wparam, lparam);
	}
}
