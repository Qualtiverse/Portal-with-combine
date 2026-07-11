#include <stdarg.h>
#include <stddef.h>

extern "C" int vsnprintf(char *, size_t, const char *, va_list);
extern "C" int vswprintf(wchar_t *, size_t, const wchar_t *, va_list);
extern "C" int vsscanf(const char *, const char *, va_list);
extern "C" int vprintf(const char *, va_list);

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
