#include "string_utils.h"
#include "platform.h"

#include <string>

namespace Storyteller
{
    namespace Utils
    {
        std::wstring NarrowToWide(const std::string& str)
        {
#if defined STRTLR_PLATFORM_WINDOWS
            const auto length = MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), 0, 0);
            std::wstring wide(length, L'\0');
            MultiByteToWideChar(CP_ACP, 0, str.c_str(), static_cast<int>(str.size()), &wide[0], length);
            return wide;
#else
            std::wstring wide;
            wide.resize(str.size() + 1);
            size_t actual;
            mbstowcs_s(&actual, wide.data(), wide.size(), str.c_str(), _TRUNCATE);
            if (actual > 0)
            {
                wide.resize(actual - 1);
                return wide;
            }

            return std::wstring{};
#endif
        }
        //--------------------------------------------------------------------------

        std::string WideToNarrow(const std::wstring& wstr)
        {
#if defined STRTLR_PLATFORM_WINDOWS
            const auto length = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
            std::string narrow(length, '\0');
            WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), static_cast<int>(wstr.size()), &narrow[0], length, nullptr, nullptr);
            return narrow;
#else
            std::string narrow;
            narrow.resize(wstr.size() * 2);
            size_t actual;
            wcstombs_s(&actual, narrow.data(), narrow.size(), wstr.c_str(), _TRUNCATE);
            narrow.resize(actual - 1);
            return narrow;
#endif
        }
        //--------------------------------------------------------------------------

        std::string NarrowToU8(const std::string& str)
        {
#if defined STRTLR_PLATFORM_WINDOWS
            const auto size = MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), static_cast<int>(str.length()), nullptr, 0);
            std::wstring u16Str(size, L'\0');
            MultiByteToWideChar(CP_ACP, MB_COMPOSITE, str.c_str(), static_cast<int>(str.length()), &u16Str[0], size);

            const auto u8Size = WideCharToMultiByte(CP_UTF8, 0, u16Str.c_str(), static_cast<int>(u16Str.length()), nullptr, 0, nullptr, nullptr);
            std::string u8Str(u8Size, '\0');
            WideCharToMultiByte(CP_UTF8, 0, u16Str.c_str(), static_cast<int>(u16Str.length()), &u8Str[0], u8Size, nullptr, nullptr);
            return u8Str;
#else
            return str;
#endif
        }
        //--------------------------------------------------------------------------

        std::string U8ToNarrow(const std::string& str)
        {
#if defined STRTLR_PLATFORM_WINDOWS
            const auto u16Size = MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
            if (u16Size > 0)
            {
                std::wstring u16Str(u16Size, L'\0');
                MultiByteToWideChar(CP_UTF8, 0, &str[0], static_cast<int>(str.size()), &u16Str[0], u16Size);
                return WideToNarrow(u16Str);
            }

            return std::string();
#else
            return str;
#endif
        }
        //--------------------------------------------------------------------------
    }
}
