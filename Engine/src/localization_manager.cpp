#include "localization_manager.h"

#include <iostream>
#include <fstream>

namespace Storyteller
{
    LocalizationManager::LocalizationManager(const std::string& defaultPath)
        : _localeGenetator()
        , _localeString("")
    {
        if (!defaultPath.empty())
        {
            _localeGenetator.add_messages_path(defaultPath);
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::Build(const std::string& localeString)
    {
        _localeString = localeString;
        std::locale::global(_localeGenetator(_localeString));
        std::cout.imbue(std::locale());
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddMessagesPath(const std::string& path)
    {
        _localeGenetator.add_messages_path(path);
    }
    //--------------------------------------------------------------------------

    void LocalizationManager::AddMessagesDomain(const std::string& domain)
    {
        _localeGenetator.add_messages_domain(domain);
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
        if (!TranslationsPathIsValid(path))
        {
            return false;
        }

        std::ofstream outputStream(path.string(), std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
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

    bool LocalizationManager::TranslationsPathIsValid(const std::filesystem::path& path) const
    {
        if (path.empty() || !path.has_filename() || !path.has_extension())
        {
            return false;
        }

        if (!std::filesystem::exists(path))
        {
            if (!std::filesystem::create_directory(path.parent_path()))
            {
                return false;
            }
        }

        return true;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}