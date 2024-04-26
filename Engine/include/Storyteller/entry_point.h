#pragma once

#include "application.h"
#include "platform.h"
#include "program_options.h"

int Main(const Storyteller::ProgramOptions& programOptions);

#if defined STRTLR_WINMAIN
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    Storyteller::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(lpCmdLine);

    return Main(programOptions);
}
//--------------------------------------------------------------------------

#else
int main(int argc, char** argv)
{
    Storyteller::ProgramOptions programOptions;
    programOptions.ProcessCommandLine(argc, argv);

    return Main(programOptions);
}
#endif
//--------------------------------------------------------------------------

int Main(const Storyteller::ProgramOptions& programOptions)
{
    Storyteller::Application* app = Storyteller::CreateApplication();
    if (!app)
    {
        return 1;
    }

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
