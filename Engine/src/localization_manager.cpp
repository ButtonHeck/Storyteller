#include "localization_manager.h"
#include "localization_library.h"
#include "filesystem.h"
#include "log.h"

#include <iostream>
#include <fstream>

namespace Storyteller
{
    LocalizationManager::LocalizationManager(const std::string& defaultPath)
        : _localeGenerator()
        , _library(new LocalizationLibrary())
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: create, default path '{}'", defaultPath);

        if (!defaultPath.empty())
        {
            _localeGenerator.add_messages_path(defaultPath);
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::SetLocale(const std::string& localeString)
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: set locale '{}'", localeString);

        std::locale::global(_localeGenerator(localeString));
        std::cout.imbue(std::locale());
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddMessagesPath(const std::string& path)
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: add messages path '{}'", path);

        _localeGenerator.add_messages_path(path);
    }
    //--------------------------------------------------------------------------

    Ptr<LocalizationDictionary> LocalizationManager::AddMessagesDomain(const std::string& domain)
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: add messages domain '{}'", domain);

        _localeGenerator.add_messages_domain(domain);
        
        return _library->AddDictionary(domain);
    }
    //--------------------------------------------------------------------------

    Ptr<LocalizationDictionary> LocalizationManager::GetDictionary(const std::string& domain) const
    {
        return _library->GetDictionary(domain);
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::CreateTranslations(const Ptr<GameDocument> document, const std::filesystem::path& path) const
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: creating translations for '{}', path '{}'", document->GetGameName(), Filesystem::ToU8String(path));

        if (!Filesystem::CreatePathTree(path))
        {
            return false;
        }

        std::ofstream outputStream(path.string(), std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            STRTLR_CORE_LOG_ERROR("LocalizationManager: error opening file stream");

            return false;
        }

        const auto gameName = document->GetGameName();
        const auto documentObjects = document->GetObjects();

        std::stringstream ss;
        ss << "Translate(\"" << gameName << "\");\n";
        for (size_t i = 0; i < documentObjects.size(); i++)
        {
            const auto object = documentObjects.at(i);
            const auto textObject = dynamic_cast<const TextObject*>(object.get());
            ss << object->GetName() << ", Translate(\"" << textObject->GetText() << "\");\n";
        }

        outputStream << ss.str();
        outputStream.close();

        return true;
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::Translate(const std::string& domain, const std::string& message)
    {
        const auto translation = LocalizationTranslator::Translate(domain, message);
        _library->Add(domain, message, translation);
        return translation;
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::Translate(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count)
    {
        return LocalizationTranslator::Translate(domain, messageSingular, messagePlural, count);
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::TranslateCtx(const std::string& domain, const std::string& message, const std::string& context)
    {
        const auto translation = LocalizationTranslator::TranslateCtx(domain, message, context);
        _library->Add(domain, message, context, translation);
        return translation;
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::TranslateCtx(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count, const std::string& context)
    {
        return LocalizationTranslator::TranslateCtx(domain, messageSingular, messagePlural, count, context);
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::Translation(const std::string& domain, const std::string& message)
    {
        return _library->Get(domain, message);
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::Translation(const std::string& domain, const std::string& message, const std::string& context)
    {
        return _library->Get(domain, message, context);
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::TranslationOr(const std::string& domain, const std::string& message, const std::string& defaultString)
    {
        const auto& translation = Translation(domain, message);
        return translation.empty() ? defaultString : translation;
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::TranslationOr(const std::string& domain, const std::string& message, const std::string& context, const std::string& defaultString)
    {
        const auto& translation = Translation(domain, message, context);
        return translation.empty() ? defaultString : translation;
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::TranslationOrSource(const std::string& domain, const std::string& message)
    {
        const auto& translation = Translation(domain, message);
        return translation.empty() ? message : translation;
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationManager::TranslationOrSource(const std::string& domain, const std::string& message, const std::string& context)
    {
        const auto& translation = Translation(domain, message, context);
        return translation.empty() ? message : translation;
    }
    //--------------------------------------------------------------------------
}