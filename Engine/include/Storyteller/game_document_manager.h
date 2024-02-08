#pragma once

#include "pointers.h"
#include "game_document.h"
#include "game_document_sort_filter_proxy_view.h"

namespace Storyteller
{
    class GameDocumentManager
    {
    public:
        GameDocumentManager();

        void NewDocument();
        void OpenDocument(const std::string& pathString);
        void OpenDocument(const std::filesystem::path& path);

        bool Load(const std::filesystem::path& path);
        bool Save();
        bool Save(const std::filesystem::path& path);

        Ptr<GameDocument> GetDocument() const;
        Ptr<GameDocumentSortFilterProxyView> GetProxy();

    private:
        Ptr<GameDocument> _document;
        Ptr<GameDocumentSortFilterProxyView> _proxy;
    };
    //--------------------------------------------------------------------------
}
