#pragma once

#include "pointers.h"
#include "game_document.h"

#include <boost/locale.hpp>

#include <map>
#include <string>

namespace Storyteller
{
    class LocalizationManager
    {
    public:
        explicit LocalizationManager(const std::string& defaultPath = "locale");

        void SetLocale(const std::string& localeString);
        void AddMessagesPath(const std::string& path);
        void AddMessagesDomain(const std::string& domain);
        std::string Translate(const std::string& domain, const std::string& message, bool noCache = false);
        std::string Translate(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count);
        bool CreateTranslations(const Ptr<GameDocument> document, const std::filesystem::path& path) const;

    private:
        boost::locale::generator _localeGenerator;
        std::string _localeString;

        typedef std::string SourceMessage;
        typedef std::string DomainName;
        typedef std::map<SourceMessage, std::string> MessagesTranslations;
        typedef std::map<DomainName, MessagesTranslations> DomainMessages;
        DomainMessages _messagesCache;
    };
    //--------------------------------------------------------------------------
}
