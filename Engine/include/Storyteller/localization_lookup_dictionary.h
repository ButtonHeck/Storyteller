#pragma once

#include "localization_base.h"

#include <unordered_map>

namespace Storyteller
{
    namespace I18N
    {
        class LookupDictionary
        {
        public:
            explicit LookupDictionary(const DomainStr& domain, const LocaleStr& defaultLocale = "");

            const DomainStr& GetDomain() const;
            void SetLocale(const LocaleStr& locale);

            void Add(const SourceStr& source, const TranslationStr& translation);
            void Add(const SourceStr& source, const ContextStr& context, const TranslationStr& translation);
            const TranslationStr& Get(const SourceStr& source);
            const TranslationStr& Get(const SourceStr& source, const ContextStr& context);

        private:
            typedef std::unordered_map<SourceStr, TranslationStr> Translations;
            typedef std::unordered_map<ContextedSource, TranslationStr, ContextedSourceHash> ContextedTranslations;
            typedef std::unordered_map<LocaleStr, Translations> LocalizedTranslations;
            typedef std::unordered_map<LocaleStr, ContextedTranslations> LocalizedContextedTranslations;

        private:
            const DomainStr _domain;
            LocaleStr _currentLocaleString;
            LocalizedTranslations _translations;
            LocalizedContextedTranslations _translationsWithContext;
        };
        //--------------------------------------------------------------------------
    }
}