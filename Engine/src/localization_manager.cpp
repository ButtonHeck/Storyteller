#include "localization_manager.h"
#include "filesystem_utils.h"
#include "log.h"

#include <iostream>
#include <fstream>

namespace Storyteller
{
    LocalizationManager::LocalizationManager(const std::string& defaultPath)
        : _localeGenerator()
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

    void LocalizationManager::AddMessagesDomain(const std::string& domain)
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: add messages domain '{}'", domain);

        _localeGenerator.add_messages_domain(domain);
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::Translate(const std::string& domain, const std::string& message)
    {
        return boost::locale::translate(message).str(domain);
    }
    //--------------------------------------------------------------------------

    std::string LocalizationManager::Translate(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count)
    {
        return (boost::locale::format(boost::locale::translate(messageSingular, messagePlural, count)) % count).str();
    }
    //--------------------------------------------------------------------------

    bool LocalizationManager::CreateTranslations(const Ptr<GameDocument> document, const std::filesystem::path& path) const
    {
        STRTLR_CORE_LOG_INFO("LocalizationManager: creating translations for '{}', path '{}'", document->GetGameName(), path.generic_string());

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
    //--------------------------------------------------------------------------
}