#pragma once

#include "windows_public.h"

#define WIN32_GUID_SIZE 16

struct _GUID;

namespace h5
{
	class guid_windows
	{
	public:
		static const guid_windows zero;

	protected:
		char value[WIN32_GUID_SIZE]{};

	public:
		guid_windows();
		guid_windows(const char *raw);
		guid_windows(e_init);
		virtual ~guid_windows();

	public:
		int32_t generate();
		void set_raw(const char *raw);

	public:
		std::string to_string() const;
		std::wstring to_wstring() const;	
		char *get_raw();
		const char *get_raw() const;

	public:
		_GUID *get_win32_guid();
		const _GUID *get_win32_guid() const;

	public:
		guid_windows &operator=(const guid_windows &other);
	};

	bool operator==(const guid_windows &v1, const guid_windows &v2);
	bool operator!=(const guid_windows &v1, const guid_windows &v2);
}
