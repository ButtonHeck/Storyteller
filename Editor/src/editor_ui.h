#pragma once

#include "game_document.h"
#include "game_document_sort_filter_proxy_view.h"
#include "localization_manager.h"
#include "window.h"

#include <memory>

namespace Storyteller
{
    class EditorUi
    {
    public:
        typedef std::shared_ptr<EditorUi> Ptr;

        EditorUi(Window::Ptr window, LocalizationManager::Ptr localizationManager);

        bool Initialize();
        void Prepare();
        void Compose();
        void Render();
        void Shutdown();

    private:
        void ComposeMenu(bool& demoWindow);
        void ComposeGameDocumentPanel();
        void ComposePropertiesPanel();
        void ComposeLogPanel();

        void AddDefaultFont();

    private:
        Window::Ptr _window;
        LocalizationManager::Ptr _localizationManager;

        GameDocument::Ptr _gameDocument;
        GameDocumentSortFilterProxyView::Ptr _gameDocumentProxy;
    };
    //--------------------------------------------------------------------------
}
