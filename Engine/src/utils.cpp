#include "utils.h"
#include "platform.h"

#include <locale>
#include <codecvt>
#include <string>

#if defined STRTLR_PLATFORM_WINDOWS
#include <conio.h>
#elif defined STRTLR_PLATFORM_LINUX
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#endif

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

        int KbHit()
        {
#if defined STRTLR_PLATFORM_WINDOWS
            return _kbhit();
#else
            int oldf, ch;

            oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
            fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

            ch = getchar();

            fcntl(STDIN_FILENO, F_SETFL, oldf);

            if (ch != EOF)
            {
                ungetc(ch, stdin);
                return 1;
            }

            return 0;
#endif
        }
        //--------------------------------------------------------------------------

        int GetCh()
        {
#if defined STRTLR_PLATFORM_WINDOWS
            return _getch();
#else
            return getchar();
#endif
        }
        //--------------------------------------------------------------------------
    }
}
