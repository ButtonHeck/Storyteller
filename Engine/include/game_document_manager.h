#pragma once

#include "game_document.h"
#include "game_document_sort_filter_proxy_view.h"

#include <memory>

namespace Storyteller
{
    class GameDocumentManager
    {
    public:
        typedef std::shared_ptr<GameDocumentManager> Ptr;

        GameDocumentManager();

        void NewDocument(const std::string& pathString = "");
        void NewDocument(const std::filesystem::path& path = "");

        GameDocument::Ptr GetDocument() const;
        GameDocumentSortFilterProxyView::Ptr GetProxy();

    private:
        GameDocument::Ptr _document;
        GameDocumentSortFilterProxyView::Ptr _proxy;
    };
    //--------------------------------------------------------------------------
}
