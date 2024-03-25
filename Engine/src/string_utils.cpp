#include "string_utils.h"
#include "platform.h"

#include <locale>
#include <codecvt>
#include <string>

namespace Storyteller
{
    namespace Utils
    {
        std::wstring FromStrBytesToWStrBytes(const std::string& strBytes)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.from_bytes(strBytes);
        }
        //--------------------------------------------------------------------------

        std::string FromWStrBytesToStrBytes(const std::wstring& wstrBytes)
        {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            return converter.to_bytes(wstrBytes);
        }
        //--------------------------------------------------------------------------
    }
}
