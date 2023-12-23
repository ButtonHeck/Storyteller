#pragma once

#include "game_document.h"

#include <boost/locale.hpp>

#include <map>
#include <string>

namespace Storyteller
{
    class LocalizationManager
    {
    public:
        typedef std::shared_ptr<LocalizationManager> Ptr;

        explicit LocalizationManager(const std::string& defaultPath = "");

        void Build(const std::string& localeString);
        void AddMessagesPath(const std::string& path);
        void AddMessagesDomain(const std::string& domain);
        std::string Translate(const std::string& domain, const std::string& message, bool noStore = false);
        bool CreateTranslations(const GameDocument::Ptr document, const std::filesystem::path& path) const;

    private:
        bool TranslationsPathIsValid(const std::filesystem::path& path) const;

    private:
        boost::locale::generator _localeGenetator;
        std::string _localeString;

        typedef std::string SourceMessage;
        typedef std::string DomainName;
        typedef std::map<SourceMessage, std::string> MessagesTranslations;
        typedef std::map<DomainName, MessagesTranslations> DomainMessages;
        DomainMessages _messages;
    };
    //--------------------------------------------------------------------------
}
