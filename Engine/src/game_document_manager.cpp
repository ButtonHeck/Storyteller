#include "game_document_manager.h"
#include "game_document_serializer.h"

namespace Storyteller
{
    GameDocumentManager::GameDocumentManager()
    {
        NewDocument();
    }
    //--------------------------------------------------------------------------

    void GameDocumentManager::NewDocument()
    {
        _document.reset(new GameDocument(std::string()));
        _proxy.reset();
    }
    //--------------------------------------------------------------------------

    bool GameDocumentManager::OpenDocument(const std::string& pathString)
    {
        return OpenDocument(std::filesystem::path(pathString));
    }
    //--------------------------------------------------------------------------

    bool GameDocumentManager::OpenDocument(const std::filesystem::path& path)
    {
        auto newDocument = CreatePtr<GameDocument>(path);
        GameDocumentSerializer serializer(newDocument);
        const auto success = serializer.Load(path);

        if (success)
        {
            _document.swap(newDocument);
            _proxy.reset();

            return true;
        }

        return false;
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