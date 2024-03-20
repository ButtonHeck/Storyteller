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
        std::ostringstream& ToSStream(std::ostringstream& ss, Ts&&... args)
        {
            (ss << ... << std::forward<Ts>(args));
            return ss;
        }
        //--------------------------------------------------------------------------

        std::wstring FromStrBytesToWStrBytes(const std::string& strBytes);
        //--------------------------------------------------------------------------

        std::string FromWStrBytesToStrBytes(const std::wstring& wstrBytes);
        //--------------------------------------------------------------------------

        #define STRTLR_BIND(func) [this](auto&& ... args) { return this->func(std::forward<decltype(args)>(args)...); }
    }
}