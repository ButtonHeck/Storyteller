#pragma once

#include "pointers.h"
#include "filesystem.h"
#include "game_document.h"
#include "localization_dictionary.h"
#include "localization_translator.h"

#include <boost/locale.hpp>

#include <string>

namespace Storyteller
{
    class LocalizationLibrary;

    class LocalizationManager
    {
    public:
        explicit LocalizationManager(const std::string& defaultPath = Filesystem::ToU8String(Filesystem::GetCurrentPath().append("locale")));

        void SetLocale(const std::string& localeString);
        void AddMessagesPath(const std::string& path);
        Ptr<LocalizationDictionary> AddMessagesDomain(const std::string& domain);
        Ptr<LocalizationDictionary> GetDictionary(const std::string& domain) const;
        bool CreateTranslations(const Ptr<GameDocument> document, const std::filesystem::path& path) const; //TODO: abstract out GameDocument

        std::string Translate(const std::string& domain, const std::string& message);
        std::string Translate(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count);
        std::string TranslateCtx(const std::string& domain, const std::string& message, const std::string& context);
        std::string TranslateCtx(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count, const std::string& context);

        static void TranslateDefer(const std::string& domain, const std::string& message) {};
        static void TranslateDefer(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count) {};
        static void TranslateCtxDefer(const std::string& domain, const std::string& message, const std::string& context) {};
        static void TranslateCtxDefer(const std::string& domain, const std::string& messageSingular, const std::string& messagePlural, int count, const std::string& context) {};

        const std::string& Translation(const std::string& domain, const std::string& message);
        const std::string& Translation(const std::string& domain, const std::string& message, const std::string& context);

    private:
        boost::locale::generator _localeGenerator;
        Ptr<LocalizationLibrary> _library;
    };
    //--------------------------------------------------------------------------
}
