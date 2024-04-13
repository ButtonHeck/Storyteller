#include "console_utils.h"
#include "platform.h"

#if defined STRTLR_PLATFORM_WINDOWS
#include <conio.h>
#elif defined STRTLR_PLATFORM_LINUX
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#endif

namespace Storyteller
{
    namespace Utils
    {
        int KbHit()
        {
#if defined STRTLR_PLATFORM_WINDOWS
            return _kbhit();
#else
            char buf = 0;
            struct termios old = { 0 };
            if (tcgetattr(0, &old) < 0)
            {
                perror("tcsetattr()");
            }

            old.c_lflag &= ~ICANON;
            old.c_lflag &= ~ECHO;
            old.c_cc[VMIN] = 1;
            old.c_cc[VTIME] = 0;

            if (tcsetattr(0, TCSANOW, &old) < 0)
            {
                perror("tcsetattr ICANON");
            }

            if (read(0, &buf, 1) < 0)
            {
                perror("read()");
            }

            old.c_lflag |= ICANON;
            old.c_lflag |= ECHO;

            if (tcsetattr(0, TCSADRAIN, &old) < 0)
            {
                perror("tcsetattr ~ICANON");
            }

            return (buf);
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
