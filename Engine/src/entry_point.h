#pragma once

#include "application.h"

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
