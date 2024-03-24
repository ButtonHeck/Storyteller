#pragma once

#include "i18n_base.h"
#include "pointers.h"

#include <unordered_map>

namespace Storyteller
{
    namespace I18N
    {
        class LookupDictionary;

        class Library
        {
        public:
            explicit Library(const LocaleStr& defaultLocale = "");

            void SetLocale(const LocaleStr& localeString);

            Ptr<LookupDictionary> AddLookupDictionary(const DomainStr& domain);
            Ptr<LookupDictionary> GetLookupDictionary(const DomainStr& domain) const;
            void RemoveLookupDictionary(const DomainStr& domain);

            void Add(const DomainStr& domain, const SourceStr& source, const TranslationStr& translation);
            void Add(const DomainStr& domain, const SourceStr& source, const ContextStr& context, const TranslationStr& translation);
            const TranslationStr& Get(const DomainStr& domain, const SourceStr& source);
            const TranslationStr& Get(const DomainStr& domain, const SourceStr& source, const ContextStr& context);

        private:
            std::unordered_map<DomainStr, Ptr<LookupDictionary>> _lookupDictionaries;
            LocaleStr _currentLocale;
        };
        //--------------------------------------------------------------------------
    }
}