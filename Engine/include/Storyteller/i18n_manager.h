#pragma once

#include "pointers.h"
#include "filesystem.h"
#include "i18n_base.h"
#include "i18n_translator.h"
#include "i18n_lookup_dictionary.h"

#include <boost/locale.hpp>

#include <functional>

namespace Storyteller
{
    namespace I18N
    {
        class Library;

        class Manager
        {
        public:
            typedef std::function<void()> LocaleChangeCallback;

        public:
            explicit Manager(const LocaleStr& defaultLocale = "", const std::string& defaultPath = Filesystem::ToString(Filesystem::GetCurrentPath().append("locale")));

            void SetLocale(const LocaleStr& localeString);
            const LocaleStr& GetLocale() const;
            void ImbueLocale() const;

            void AddMessagesPath(const std::string& path);

            Ptr<LookupDictionary> AddMessagesDomain(const DomainStr& domain);
            void RemoveMessagesDomain(const DomainStr& domain);

            Ptr<LookupDictionary> GetLookupDictionary(const DomainStr& domain) const;

            void AddLocaleChangedCallback(const LocaleChangeCallback& callback);

            TranslationStr Translate(const DomainStr& domain, const SourceStr& message);
            TranslationStr Translate(const DomainStr& domain, const SourceStr& messageSingular, const SourceStr& messagePlural, int count);
            TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& message, const ContextStr& context);
            TranslationStr TranslateCtx(const DomainStr& domain, const SourceStr& messageSingular, const SourceStr& messagePlural, int count, const ContextStr& context);

            static void TranslateDefer(const DomainStr& domain, const SourceStr& message) {};
            static void TranslateDefer(const DomainStr& domain, const SourceStr& messageSingular, const SourceStr& messagePlural, int count) {};
            static void TranslateCtxDefer(const DomainStr& domain, const SourceStr& message, const ContextStr& context) {};
            static void TranslateCtxDefer(const DomainStr& domain, const SourceStr& messageSingular, const SourceStr& messagePlural, int count, const ContextStr& context) {};

            const TranslationStr& Translation(const DomainStr& domain, const SourceStr& message);
            const TranslationStr& Translation(const DomainStr& domain, const SourceStr& message, const ContextStr& context);
            const TranslationStr& TranslationOr(const DomainStr& domain, const SourceStr& message, const TranslationStr& defaultString);
            const TranslationStr& TranslationOr(const DomainStr& domain, const SourceStr& message, const ContextStr& context, const TranslationStr& defaultString);
            const TranslationStr& TranslationOrSource(const DomainStr& domain, const SourceStr& message);
            const TranslationStr& TranslationOrSource(const DomainStr& domain, const SourceStr& message, const ContextStr& context);

        private:
            void NotifyLocaleListeners() const;

        private:
            boost::locale::generator _localeGenerator;
            Ptr<Library> _library;
            LocaleStr _currentLocale;
            std::vector<LocaleChangeCallback> _localeChangedCallbacks;
        };
        //--------------------------------------------------------------------------
    }
}
