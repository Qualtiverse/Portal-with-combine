#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#include <new>

extern "C" void *__cdecl malloc(size_t s) { return HeapAlloc(GetProcessHeap(), 0, s); }
extern "C" void __cdecl free(void *p) { if (p) HeapFree(GetProcessHeap(), 0, p); }

void *operator new(size_t s) { void *p = HeapAlloc(GetProcessHeap(), 0, s); if (!p && s) return NULL; return p; }
void *operator new(size_t s, const std::nothrow_t &) noexcept { return HeapAlloc(GetProcessHeap(), 0, s); }
void operator delete(void *p) noexcept { if (p) HeapFree(GetProcessHeap(), 0, p); }
void operator delete(void *p, size_t) noexcept { if (p) HeapFree(GetProcessHeap(), 0, p); }
void *operator new[](size_t s) { return operator new(s); }
void operator delete[](void *p) noexcept { if (p) HeapFree(GetProcessHeap(), 0, p); }
void operator delete[](void *p, size_t) noexcept { if (p) HeapFree(GetProcessHeap(), 0, p); }
