#pragma once
#define sassert(expr) static_assert(expr, #expr)

#define CAT_(a, b) a##b
#define CAT(a, b)  CAT_(a, b)

#define PRAGMA(x) _Pragma(#x)
#if defined(__clang__)
#define CLANG_GCC_DISABLE_WARNING_PUSH(warning) PRAGMA(clang diagnostic push) \
    PRAGMA(clang diagnostic ignored warning)
#define CLANG_GCC_DISABLE_WARNING_POP PRAGMA(clang diagnostic pop)
#define NO_WARNINGS_PUSH  CLANG_GCC_DISABLE_WARNING_PUSH("-Weverything")
#define NO_WARNINGS_POP CLANG_GCC_DISABLE_WARNING_POP
#elif defined(_MSC_VER)
#define CLANG_GCC_DISABLE_WARNING_PUSH(warning)
#define CLANG_GCC_DISABLE_WARNING_POP
#define NO_WARNINGS_PUSH  _Pragma("warning(push, 0)")
#define NO_WARNINGS_POP _Pragma("warning(pop)")
#else
#define CLANG_GCC_DISABLE_WARNING_PUSH(warning) PRAGMA(gcc diagnostic push) \
    PRAGMA(gcc diagnostic ignored #warning)
#define CLANG_GCC_DISABLE_WARNING_POP _Pragma(gcc diagnostic pop)
#define NO_WARNINGS_PUSH  CLANG_GCC_DISABLE_WARNING_PUSH("-Weverything")
#define NO_WARNINGS_POP CLANG_GCC_DISABLE_WARNING_POP
#endif
