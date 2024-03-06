#pragma once

#include "pointers.h"
#include "filesystem.h"
#include "game_document.h"

#include <boost/locale.hpp>

#include <string>

namespace Storyteller
{
    class LocalizationManager
    {
    public:
        explicit LocalizationManager(const std::string& defaultPath = Filesystem::ToU8String(Filesystem::GetCurrentPath().append("locale")));

        void SetLocale(const std::string& localeString);
        void AddMessagesPath(const std::string& path);
        void AddMessagesDomain(const std::string& domain);
        std::string Translate(const std::string& domain, const std::string& message);
        std::string Translate(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count);
        std::string TranslateCtx(const std::string& domain, const std::string& message, const std::string& context);
        std::string TranslateCtx(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count, const std::string& context);
        bool CreateTranslations(const Ptr<GameDocument> document, const std::filesystem::path& path) const;

    private:
        boost::locale::generator _localeGenerator;
    };
    //--------------------------------------------------------------------------
}
