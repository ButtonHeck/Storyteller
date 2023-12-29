#pragma once

#include "application.h"

namespace Storyteller
{
    class EditorApplication : public Application
    {
    public:
        EditorApplication() = default;

        virtual ~EditorApplication() = default;

        bool Initialize() override;
        void Run() override;

    private:
    };
}
