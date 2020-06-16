#include "debug.h"

#include <cstring>

const char * get_filename(const char *str)
{
	size_t length = strlen(str);
	for (size_t i = length; i > 0; --i)
	{
		if (str[i] == '\\')
		{
			return &str[i];
		}
	}
	return str;
}
