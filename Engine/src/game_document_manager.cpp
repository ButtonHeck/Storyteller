#include "game_document_manager.h"
#include "game_document_serializer.h"
#include "localization_manager.h"
#include "filesystem.h"
#include "log.h"
#include "utils.h"

#include <fstream>

namespace Storyteller
{
    GameDocumentManager::GameDocumentManager(Ptr<LocalizationManager> localizationManager)
        : _localizationManager(localizationManager)
    {
        _localizationManager->AddLocaleChangedCallback(STRTLR_BIND(GameDocumentManager::FillDictionary));

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
            _localizationManager->RemoveMessagesDomain(_document->GetDomainName());

            _document.swap(newDocument);
            _proxy.reset();

            _localizationManager->AddMessagesPath(Filesystem::ToU8String(_document->GetTranslationsPath()));
            _localizationManager->AddMessagesDomain(_document->GetDomainName());
            FillDictionary();

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

    bool GameDocumentManager::CreateTranslations(const std::filesystem::path& path) const
    {
        STRTLR_CORE_LOG_INFO("GameDocumentManager: creating translations for '{}', path '{}'", _document->GetGameName(), Filesystem::ToU8String(path));

        if (!Filesystem::CreatePathTree(path))
        {
            STRTLR_CORE_LOG_ERROR("GameDocumentManager: cannot create path '{}' for translations");

            return false;
        }

        std::ofstream outputStream(path.string(), std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            STRTLR_CORE_LOG_ERROR("GameDocumentManager: error opening file stream");

            return false;
        }

        const auto gameName = _document->GetGameName();
        const auto documentObjects = _document->GetObjects();

        std::ostringstream ss;
        const auto openBracket = '(';
        const auto closeBracket = ')';
        const auto quote = '\"';
        const auto separator = ", ";
        const auto semicolon = ';';
        const auto newLine = '\n';

        Utils::ToSStream(ss, "Game name", newLine);
        Utils::ToSStream(ss, LocalizationManager::TranslateKeyword, openBracket, quote, gameName, quote, closeBracket, semicolon, newLine);
        for (auto i = 0; i < documentObjects.size(); i++)
        {
            const auto object = documentObjects.at(i);
            const auto textObject = dynamic_cast<const TextObject*>(object.get());

            Utils::ToSStream(ss, object->GetName(), newLine);
            Utils::ToSStream(ss, LocalizationManager::TranslateKeyword, openBracket, quote, textObject->GetText(), quote, closeBracket, semicolon, newLine);
        }

        outputStream << ss.str();
        outputStream.close();

        return true;
    }
    //--------------------------------------------------------------------------

    void GameDocumentManager::FillDictionary() const
    {
        _localizationManager->Translate(_document->GetDomainName(), _document->GetGameName());

        const auto objects = _document->GetObjects<TextObject>();
        for (const auto& object : objects)
        {
            _localizationManager->Translate(_document->GetDomainName(), object->GetText());
        }
    }
    //--------------------------------------------------------------------------
}