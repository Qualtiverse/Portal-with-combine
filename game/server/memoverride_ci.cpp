#define WIN_32_LEAN_AND_MEAN
#include <windows.h>
#include <new>

// Override CRT allocation to use process heap (same heap as engine's g_pMemAlloc)

extern "C" {

static void *Alloc(size_t s) { return HeapAlloc(GetProcessHeap(), 0, s); }
static void Free(void *p) { if (p) HeapFree(GetProcessHeap(), 0, p); }

void *__cdecl malloc(size_t s) { return Alloc(s); }
void __cdecl free(void *p) { Free(p); }
void *__cdecl calloc(size_t n, size_t s) { size_t t = n * s; void *p = Alloc(t); if (p) ZeroMemory(p, t); return p; }
void *__cdecl realloc(void *p, size_t s) { if (!p) return Alloc(s); if (!s) { Free(p); return NULL; } return HeapReAlloc(GetProcessHeap(), 0, p, s); }

} // extern "C"

void *operator new(size_t s) { void *p = Alloc(s); if (!p && s) return NULL; return p; }
void *operator new(size_t s, const std::nothrow_t &) noexcept { return Alloc(s); }
void operator delete(void *p) noexcept { Free(p); }
void operator delete(void *p, size_t) noexcept { Free(p); }
void *operator new[](size_t s) { return operator new(s); }
void operator delete[](void *p) noexcept { Free(p); }
void operator delete[](void *p, size_t) noexcept { Free(p); }
