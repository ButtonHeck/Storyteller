#pragma once

#include "pointers.h"
#include "application.h"
#include "window.h"

namespace Storyteller
{
    class WindowApplication : public Application
    {
    public:
        WindowApplication();

        bool Initialize() override;

    protected:
        Ptr<Window> _window;
    };
    //--------------------------------------------------------------------------
}