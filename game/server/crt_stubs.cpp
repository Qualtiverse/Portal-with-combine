#include <stdarg.h>
#include <stddef.h>

extern "C" int __cdecl _vsnprintf_l(char *, size_t, const char *, va_list, void *);
extern "C" int __cdecl _vsnwprintf_l(wchar_t *, size_t, const wchar_t *, va_list, void *);
extern "C" int __cdecl _vswprintf_c_l(wchar_t *, size_t, const wchar_t *, va_list, void *);
extern "C" int __cdecl _vprintf_l(const char *, void *, va_list);
extern "C" int __cdecl _vsscanf_l(const char *, const char *, void *, va_list);

extern "C"
{
	int __cdecl _vsnprintf(char *buffer, size_t count, const char *format, va_list argptr)
	{
		return _vsnprintf_l(buffer, count, format, argptr, NULL);
	}

	int __cdecl _vsnwprintf(wchar_t *buffer, size_t count, const wchar_t *format, va_list argptr)
	{
		return _vsnwprintf_l(buffer, count, format, argptr, NULL);
	}

	int __cdecl printf(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		int result = _vprintf_l(format, NULL, args);
		va_end(args);
		return result;
	}

	int __cdecl sscanf_s(const char *buffer, const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		int result = _vsscanf_l(buffer, format, NULL, args);
		va_end(args);
		return result;
	}

	int __cdecl vswprintf(wchar_t *buffer, size_t count, const wchar_t *format, va_list argptr)
	{
		return _vswprintf_c_l(buffer, count, format, argptr, NULL);
	}

	int __cdecl _vswprintf(wchar_t *buffer, wchar_t *format, va_list argptr)
	{
		return _vswprintf_c_l(buffer, (size_t)-1, format, argptr, NULL);
	}
}
