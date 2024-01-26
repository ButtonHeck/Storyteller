#include "localization_manager.h"
#include "filesystem_utils.h"
#include "log.h"

#include <iostream>
#include <fstream>

namespace Storyteller
{
    LocalizationManager::LocalizationManager(const std::string& defaultPath)
        : _localeGenerator()
        , _localeString("")
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: create, default path '{}'", defaultPath);

        if (!defaultPath.empty())
        {
            _localeGenerator.add_messages_path(defaultPath);
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::Build(const std::string& localeString)
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: building locale '{}'", localeString);

        _localeString = localeString;
        std::locale::global(_localeGenerator(_localeString));
        std::cout.imbue(std::locale());
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddMessagesPath(const std::string& path)
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: add messages path '{}'", path);

        _localeGenerator.add_messages_path(path);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddMessagesDomain(const std::string& domain)
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: add messages domain '{}'", domain);

        _localeGenerator.add_messages_domain(domain);
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::Translate(const std::string& domain, const std::string& message, bool noStore)
    {
        if (_messages.contains(domain))
        {
            const auto& domainTranslations = _messages[domain];
            if (domainTranslations.contains(message))
            {
                return domainTranslations.at(message);
            }
        }

        const auto translation = boost::locale::translate(message).str(domain);
        if (translation == message && noStore)
        {
            return "No translation";
        }

        _messages[domain][message] = translation;

        return translation;
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::CreateTranslations(const GameDocument::Ptr document, const std::filesystem::path& path) const
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: creating translations for '{}', path '{}'", document->GetGameName(), path.generic_string());

        if (!Filesystem::CheckPathAndTryCreate(path))
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
        ss << "Translate(\"" << gameName << "\", \"" << gameName << "\");\n";
        for (size_t i = 0; i < documentObjects.size(); i++)
        {
            const auto object = documentObjects.at(i);
            const auto textObject = dynamic_cast<const TextObject*>(object.get());
            ss << object->GetName() << ", Translate(\"" << gameName << "\", \"" << textObject->GetText() << "\");\n";
        }

        outputStream << ss.str();
        outputStream.close();

        return true;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}