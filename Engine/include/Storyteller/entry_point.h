#pragma once

#include "application.h"
#include "platform.h"
#include "program_options.h"

#if defined STRTLR_WINMAIN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Storyteller::Application* app = Storyteller::CreateApplication();
    if (!app)
    {
        return 1;
    }

    Storyteller::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(lpCmdLine);

    if (!app->Initialize(programOptions.GetConfigPath()))
    {
        delete app;
        return 2;
    }

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
        return 1;
    }

    Storyteller::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(argc, argv);

    if (!app->Initialize(programOptions.GetConfigPath()))
    {
        delete app;
        return 2;
    }

    app->Run();

    delete app;

    return 0;
}
#endif
//--------------------------------------------------------------------------
