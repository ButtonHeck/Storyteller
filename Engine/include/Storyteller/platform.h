#pragma once

#if defined (_WIN32) || defined (_WIN64)
    #define STRTLR_PLATFORM_WINDOWS
    #define NOGDI
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    #define STRTLR_PLATFORM_LINUX
#elif define (__APPLE__)
    #define STRTLR_PLATFORM_MACOSX
#endif

#if defined (_MSC_VER)
    #define STRTLR_COMPILER_MSVC
#elif defined (__MINGW32__) || defined (__MINGW64__)
    #define STRTLR_COMPILER_MINGW
#elif defined (__GNUC__)
    #define STRTLR_COMPILER_GCC
#endif