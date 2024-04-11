#pragma once

#include <sstream>

namespace Storyteller
{
    namespace Utils
    {
        template<typename... Ts>
        std::string Concatenate(Ts&&... args)
        {
            std::ostringstream oss;
            (oss << ... << std::forward<Ts>(args));
            return oss.str();
        }
        //--------------------------------------------------------------------------

        template<typename... Ts>
        std::ostringstream& ToSStream(std::ostringstream& oss, Ts&&... args)
        {
            (oss << ... << std::forward<Ts>(args));
            return oss;
        }
        //--------------------------------------------------------------------------

        std::wstring NarrowToWide(const std::string& str);
        //--------------------------------------------------------------------------

        std::string WideToNarrow(const std::wstring& wstr);
        //--------------------------------------------------------------------------

        std::string NarrowToU8(const std::string& str);
        //--------------------------------------------------------------------------

        std::string U8ToNarrow(const std::string& str);
        //--------------------------------------------------------------------------
    }
}