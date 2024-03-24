#pragma once

#include "pointers.h"
#include "game_document.h"
#include "game_document_sort_filter_proxy_view.h"
#include "i18n_manager.h"

namespace Storyteller
{
    class GameDocumentManager
    {
    public:
        explicit GameDocumentManager(const Ptr<I18N::Manager> i18nManager);

        void NewDocument();
        bool OpenDocument(const std::filesystem::path& path);

        bool Load(const std::filesystem::path& path);
        bool Save() const;
        bool Save(const std::filesystem::path& path) const;

        Ptr<GameDocument> GetDocument() const;
        Ptr<GameDocumentSortFilterProxyView> GetProxy();

        bool CreateTranslations(const std::filesystem::path& path) const;

    private:
        void FillDictionary() const;

    private:
        const Ptr<I18N::Manager> _i18nManager;
        Ptr<GameDocument> _document;
        Ptr<GameDocumentSortFilterProxyView> _proxy;
    };
    //--------------------------------------------------------------------------
}
