#include "game_document_manager.h"
#include "game_document_serializer.h"

namespace Storyteller
{
    GameDocumentManager::GameDocumentManager()
    {
        NewDocument(std::string());
    }
    //--------------------------------------------------------------------------

    void GameDocumentManager::NewDocument(const std::string& pathString)
    {
        NewDocument(std::filesystem::path(pathString));
    }
    //--------------------------------------------------------------------------

    void GameDocumentManager::NewDocument(const std::filesystem::path& path)
    {
        _document.reset(new GameDocument(path));

        GameDocumentSerializer serializer(_document);
        serializer.Load(path);

        _proxy.reset();
    }
    //--------------------------------------------------------------------------

    bool GameDocumentManager::Load(const std::filesystem::path& path)
    {
        GameDocumentSerializer serializer(_document);
        return serializer.Load(path);
    }
    //--------------------------------------------------------------------------

    bool GameDocumentManager::Save()
    {
        GameDocumentSerializer serializer(_document);
        return serializer.Save();
    }
    //--------------------------------------------------------------------------

    bool GameDocumentManager::Save(const std::filesystem::path& path)
    {
        GameDocumentSerializer serializer(_document);
        return serializer.Save(path);
    }
    //--------------------------------------------------------------------------

    Ptr<GameDocument> GameDocumentManager::GetDocument() const
    {
        return _document;
    }
    //--------------------------------------------------------------------------

    Ptr<GameDocumentSortFilterProxyView> GameDocumentManager::GetProxy()
    {
        if (!_proxy)
        {
            _proxy.reset(new GameDocumentSortFilterProxyView(_document));
        }

        return _proxy;
    }
    //--------------------------------------------------------------------------
}