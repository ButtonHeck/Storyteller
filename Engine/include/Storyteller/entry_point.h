#pragma once

#include "application.h"
#include "platform.h"

#if defined STRTLR_WINMAIN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
    Storyteller::Application* app = Storyteller::CreateApplication();
    if (!app)
    {
        return -1;
    }

    app->Initialize();
    app->Run();

    delete app;

    return 0;
}
//--------------------------------------------------------------------------

#else
int main(int argc, char** argv)
{
    Storyteller::Application* app = Storyteller::CreateApplication();
    if (!app)
    {
        return -1;
    }

    app->Initialize();
    app->Run();

    delete app;

    return 0;
}
#endif
//--------------------------------------------------------------------------
