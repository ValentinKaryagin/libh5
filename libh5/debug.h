#pragma once

#include <cstdio>

const char * get_filename(const char *str);

#ifdef _DEBUG
#define DEBUG_PRINT_FUNC()					printf("DEBUG: " __FUNCTION__ "\n")
#define DEBUG_PRINTF(text, ...)				printf("DEBUG: " text "\n", __VA_ARGS__)
#define DEBUG_WPRINTF(text, ...)			wprintf(u("DEBUG: ") text u("\n"), __VA_ARGS__)
#define DEBUG_PRINT_FILELINE()				printf("DEBUG: File %s Line %d\n", get_filename(__FILE__), __LINE__)
#define DEBUG_PRINT_FILELINERES(res)		printf("DEBUG: File %s Line %d Result %d\n", get_filename(__FILE__), __LINE__, res)
#define DEBUG_PRINT_FUNCFILELINE()			printf("DEBUG: %s File %s Line %d\n", __FUNCTION__, get_filename(__FILE__), __LINE__)
#define DEBUG_PRINT_FUNCFILELINERES(res)	printf("DEBUG: %s File %s Line %d Result %d\n", __FUNCTION__, get_filename(__FILE__), __LINE__, res)
#else
#define DEBUG_PRINT_FUNC()
#define DEBUG_PRINTF(text, ...)
#define DEBUG_PRINT_FILELINE()
#define DEBUG_PRINT_FILELINERES(res)
#define DEBUG_PRINT_FUNCFILELINE()
#define DEBUG_PRINT_FUNCFILELINERES(res)
#endif
