#include <stdarg.h>
#include <stddef.h>

extern "C" int __cdecl _vswprintf_c_l(wchar_t *, size_t, const wchar_t *, va_list, void *);

extern "C"
{
	int __cdecl vswprintf(wchar_t *buffer, size_t count, const wchar_t *format, va_list argptr)
	{
		return _vswprintf_c_l(buffer, count, format, argptr, NULL);
	}

	int __cdecl _vswprintf(wchar_t *buffer, wchar_t *format, va_list argptr)
	{
		return _vswprintf_c_l(buffer, (size_t)-1, format, argptr, NULL);
	}
}
