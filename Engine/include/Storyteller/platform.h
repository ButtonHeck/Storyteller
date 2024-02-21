#pragma once

#if defined (_WIN32) || defined (_WIN64)
    #define STRTLR_PLATFORM_WINDOWS
    #define NOGDI
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif
