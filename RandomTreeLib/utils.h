#ifndef UTILS_H_INCLUDE_GUARD
#define UTILS_H_INCLUDE_GUARD
#include <string.h>
#include <stdarg.h>

#define STRIFY(x) #x
#define XSTRIFY(x) STRIFY(x)

typedef unsigned int uint;
typedef char**const void_tab_ptr;
typedef char**const void_ptr_ref;
typedef void(* deleter_func)(void*);

void* MemCopyAlloc(const void* source, const size_t size);

void FreeTab(void_tab_ptr tab, const size_t count);

void* CreateTab(const size_t first, const size_t second, const size_t typeSize);

void FreeTab_Func(void_tab_ptr tab, const size_t count, deleter_func deleter);

void FreeN(void_ptr_ref ptr);  // NOLINT

inline size_t MemSizeChars(const char* source)
{
	return sizeof(char) * (strlen(source) + 1);
}

inline char* MemCopyChars(const char* source)
{
	return (char*)MemCopyAlloc(source, MemSizeChars(source));
}

char* MemCopyCharsNoEnter(const char* source);

#ifdef _DEBUG
#define DBG_PRINT(x, ...) \
	{\
		printf_s((const char*)(strrchr("\\#"__FILE__ ":" XSTRIFY(__LINE__)" DBG: ", '\\') + 1));\
		printf(x, __VA_ARGS__);\
	}
#else
#define DBG_PRINT(x) /*nothing*/
#endif
#endif

#define _FreeN(ptr_ref) FreeN((void_ptr_ref)ptr_ref)

#define NULLCHECK(func, vName) vName = func; if (vName == NULL) return NULL;
#define NULLCHECK_V(func, vName) vName = func; if (vName == NULL) return;

#define _calloc(count, size, vName) NULLCHECK(calloc(count, size), vName)
#define _calloc_v(count, size, vName) NULLCHECK_V(calloc(count, size), vName)

#define _malloc(size, vName) NULLCHECK(malloc(size), vName)
#define _malloc_v(size, vName) NULLCHECK_V(malloc(size), vName)