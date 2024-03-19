#pragma once

#include "pointers.h"
#include "filesystem.h"
#include "localization_translator.h"
#include "localization_lookup_dictionary.h"

#include <boost/locale.hpp>

#include <string>
#include <functional>

namespace Storyteller
{
    class LocalizationLibrary;

    class LocalizationManager
    {
    public:
        static constexpr auto TranslateKeyword = "Translate";
        static constexpr auto TranslateCtxKeyword = "TranslateCtx";
        static constexpr auto TranslateDeferKeyword = "TranslateDefer";
        static constexpr auto TranslateCtxDeferKeyword = "TranslateCtxDefer";

        static constexpr auto LocaleEnUTF8Keyword = "en_EN.UTF-8";
        static constexpr auto LocaleEnName = "English";
        static constexpr auto LocaleRuUTF8Keyword = "ru_RU.UTF-8";
        static constexpr auto LocaleRuName = u8"Русский";

    public:
        typedef std::function<void()> LocaleChangeCallback;

    public:
        explicit LocalizationManager(const std::string& defaultLocale = "", const std::string& defaultPath = Filesystem::ToString(Filesystem::GetCurrentPath().append("locale")));

        void SetLocale(const std::string& localeString);
        const std::string& GetLocale() const;
        void ImbueLocale() const;
        void AddMessagesPath(const std::string& path);
        Ptr<LocalizationLookupDictionary> AddMessagesDomain(const std::string& domain);
        Ptr<LocalizationLookupDictionary> GetLookupDictionary(const std::string& domain) const;
        void AddLocaleChangedCallback(const LocaleChangeCallback& callback);

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
        const std::string& TranslationOr(const std::string& domain, const std::string& message, const std::string& defaultString);
        const std::string& TranslationOr(const std::string& domain, const std::string& message, const std::string& context, const std::string& defaultString);
        const std::string& TranslationOrSource(const std::string& domain, const std::string& message);
        const std::string& TranslationOrSource(const std::string& domain, const std::string& message, const std::string& context);

    private:
        void NotifyLocaleListeners() const;

    private:
        boost::locale::generator _localeGenerator;
        Ptr<LocalizationLibrary> _library;
        std::string _currentLocaleString;
        std::vector<LocaleChangeCallback> _localeChangedCallbacks;
    };
    //--------------------------------------------------------------------------
}
