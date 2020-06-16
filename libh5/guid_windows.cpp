#include "guid_windows.h"

#include <guiddef.h>
#include <Windows.h>

#include <cstdio>

namespace h5
{
	constexpr size_t GUID_STR_SIZE = 37;

	guid_windows::guid_windows() {}

	guid_windows::guid_windows(const char *raw)
	{
		memcpy(value, raw, WIN32_GUID_SIZE);
	}

	guid_windows::guid_windows(e_init)
	{
		CoCreateGuid((GUID *)value);
	}

	guid_windows::~guid_windows() {}

	int32_t guid_windows::generate()
	{
		return SUCCEEDED(CoCreateGuid((GUID *)value)) ? 0 : ERRLVL_WIN32_GENERATE_GUID;
	}

	void guid_windows::set_raw(const char *raw)
	{
		memcpy(value, raw, WIN32_GUID_SIZE);
	}

	std::string guid_windows::to_string() const
	{
		char guid_str[GUID_STR_SIZE]{};
#ifdef _MSC_VER
		sprintf_s(
#else
		sprintf(
#endif
			guid_str,
			"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			((GUID *)value)->Data1,
			((GUID *)value)->Data2,
			((GUID *)value)->Data3,
			((GUID *)value)->Data4[0],
			((GUID *)value)->Data4[1],
			((GUID *)value)->Data4[2],
			((GUID *)value)->Data4[3],
			((GUID *)value)->Data4[4],
			((GUID *)value)->Data4[5],
			((GUID *)value)->Data4[6],
			((GUID *)value)->Data4[7]);
		return guid_str;
	}

	std::wstring guid_windows::to_wstring() const
	{
		wchar_t guid_str[GUID_STR_SIZE]{};
		wsprintfW(
			guid_str,
			u("%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x"),
			((GUID *)value)->Data1,
			((GUID *)value)->Data2,
			((GUID *)value)->Data3,
			((GUID *)value)->Data4[0],
			((GUID *)value)->Data4[1],
			((GUID *)value)->Data4[2],
			((GUID *)value)->Data4[3],
			((GUID *)value)->Data4[4],
			((GUID *)value)->Data4[5],
			((GUID *)value)->Data4[6],
			((GUID *)value)->Data4[7]);
		return guid_str;
	}

	char *guid_windows::get_raw()
	{
		return value;
	}

	const char *guid_windows::get_raw() const
	{
		return value;
	}

	_GUID *guid_windows::get_win32_guid()
	{
		return (_GUID *)value;
	}

	const _GUID *guid_windows::get_win32_guid() const
	{
		return (_GUID *)value;
	}

	guid_windows &guid_windows::operator=(const guid_windows &other)
	{
		if (this != &other)
		{
			memcpy(value, other.value, WIN32_GUID_SIZE);
		}
		return *this;
	}

	bool operator==(const guid_windows &v1, const guid_windows &v2)
	{
		return memcmp(v1.get_raw(), v2.get_raw(), WIN32_GUID_SIZE) == 0;
	}

	bool operator!=(const guid_windows &v1, const guid_windows &v2)
	{
		return memcmp(v1.get_raw(), v2.get_raw(), WIN32_GUID_SIZE) != 0;
	}
}
