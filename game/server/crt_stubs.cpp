#include "cbase.h"
#include <stdio.h>
#include <stdarg.h>

extern "C"
{
	int __cdecl _vsnprintf(char *buffer, size_t count, const char *format, va_list argptr)
	{
		return vsnprintf(buffer, count, format, argptr);
	}

	int __cdecl _vsnwprintf(wchar_t *buffer, size_t count, const wchar_t *format, va_list argptr)
	{
		return vswprintf(buffer, count, format, argptr);
	}

	int __cdecl printf(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		int result = vprintf(format, args);
		va_end(args);
		return result;
	}

	int __cdecl sscanf_s(const char *buffer, const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		int result = vsscanf(buffer, format, args);
		va_end(args);
		return result;
	}
}
