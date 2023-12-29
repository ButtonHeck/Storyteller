#include "application.h"
#include "log.h"

namespace Storyteller
{
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