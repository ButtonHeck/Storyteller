#include "game_document_manager.h"

namespace Storyteller
{
    GameDocumentManager::GameDocumentManager()
    {
        NewDocument(std::string());
    }
    //--------------------------------------------------------------------------

    void GameDocumentManager::NewDocument(const std::string& pathString)
    {
        _document.reset(new GameDocument(pathString));
        _proxy.reset();
    }
    //--------------------------------------------------------------------------

    void GameDocumentManager::NewDocument(const std::filesystem::path& path)
    {
        _document.reset(new GameDocument(path));
        _proxy.reset();
    }
    //--------------------------------------------------------------------------

    GameDocument::Ptr GameDocumentManager::GetDocument() const
    {
        return _document;
    }
    //--------------------------------------------------------------------------

    GameDocumentSortFilterProxyView::Ptr GameDocumentManager::GetProxy()
    {
        if (!_proxy)
        {
            _proxy.reset(new GameDocumentSortFilterProxyView(_document));
        }

        return _proxy;
    }
    //--------------------------------------------------------------------------
}