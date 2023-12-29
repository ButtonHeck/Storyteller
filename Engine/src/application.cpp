#include "application.h"
#include "log.h"

namespace Storyteller
{
    Application::Application()
        : _localizationManager(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool Application::Initialize()
    {
        Log::Initialize();

        _localizationManager.reset(new LocalizationManager());

        return true;
    }
    //--------------------------------------------------------------------------

    void Application::Run()
    {
    }
    //--------------------------------------------------------------------------
}