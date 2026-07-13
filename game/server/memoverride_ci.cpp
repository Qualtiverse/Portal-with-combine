#define _ACRTIMP
#define _CRTIMP
#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#include <new>
#include <cstdlib>

void *__cdecl malloc(size_t s) { void *p = HeapAlloc(GetProcessHeap(), 0, s); return p; }
void __cdecl free(void *p) { if (p) HeapFree(GetProcessHeap(), 0, p); }
void *__cdecl calloc(size_t n, size_t s) { size_t ns = n * s; void *p = HeapAlloc(GetProcessHeap(), 0, ns); if (p) ZeroMemory(p, ns); return p; }
void *__cdecl realloc(void *p, size_t s) { if (!p) return malloc(s); if (!s) { free(p); return NULL; } return HeapReAlloc(GetProcessHeap(), 0, p, s); }

void *operator new(size_t s) { void *p = HeapAlloc(GetProcessHeap(), 0, s); if (!p && s) return NULL; return p; }
void *operator new(size_t s, const std::nothrow_t &) noexcept { return HeapAlloc(GetProcessHeap(), 0, s); }
void operator delete(void *p) noexcept { if (p) HeapFree(GetProcessHeap(), 0, p); }
void operator delete(void *p, size_t) noexcept { if (p) HeapFree(GetProcessHeap(), 0, p); }
void *operator new[](size_t s) { return operator new(s); }
void operator delete[](void *p) noexcept { if (p) HeapFree(GetProcessHeap(), 0, p); }
void operator delete[](void *p, size_t) noexcept { if (p) HeapFree(GetProcessHeap(), 0, p); }
