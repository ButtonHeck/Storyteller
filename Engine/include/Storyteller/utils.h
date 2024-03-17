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

        std::wstring FromStrBytesToWStrBytes(const std::string& strBytes);
        //--------------------------------------------------------------------------

        std::string FromWStrBytesToStrBytes(const std::wstring& wstrBytes);
        //--------------------------------------------------------------------------
    }
}