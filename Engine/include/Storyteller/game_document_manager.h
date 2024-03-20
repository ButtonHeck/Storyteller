#pragma once

#include "pointers.h"
#include "game_document.h"
#include "game_document_sort_filter_proxy_view.h"
#include "localization_manager.h"

namespace Storyteller
{
    class GameDocumentManager
    {
    public:
        explicit GameDocumentManager(Ptr<LocalizationManager> localizationManager);

        void NewDocument();
        bool OpenDocument(const std::string& pathString);
        bool OpenDocument(const std::filesystem::path& path);

        bool Load(const std::filesystem::path& path);
        bool Save();
        bool Save(const std::filesystem::path& path);

        Ptr<GameDocument> GetDocument() const;
        Ptr<GameDocumentSortFilterProxyView> GetProxy();

        bool CreateTranslations(const std::filesystem::path& path) const;

    private:
        void FillDictionary() const;

    private:
        Ptr<GameDocument> _document;
        Ptr<GameDocumentSortFilterProxyView> _proxy;
        Ptr<LocalizationManager> _localizationManager;
    };
    //--------------------------------------------------------------------------
}
