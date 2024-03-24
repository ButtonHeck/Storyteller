#include "i18n_lookup_dictionary.h"

namespace Storyteller
{
    namespace I18N
    {
        LookupDictionary::LookupDictionary(const DomainStr& domain, const LocaleStr& defaultLocale)
            : _domain(domain)
            , _currentLocaleString(defaultLocale)
        {}
        //--------------------------------------------------------------------------

        const DomainStr& LookupDictionary::GetDomain() const
        {
            return _domain;
        }
        //--------------------------------------------------------------------------

        void LookupDictionary::SetLocale(const LocaleStr& locale)
        {
            _currentLocaleString = locale;
        }
        //--------------------------------------------------------------------------

        void LookupDictionary::Add(const SourceStr& source, const TranslationStr& translation)
        {
            _translations[_currentLocaleString].insert(std::make_pair(source, translation));
        }
        //--------------------------------------------------------------------------

        void LookupDictionary::Add(const SourceStr& source, const ContextStr& context, const TranslationStr& translation)
        {
            _translationsWithContext[_currentLocaleString].insert(std::make_pair(ContextedSource(source, context), translation));
        }
        //--------------------------------------------------------------------------

        const TranslationStr& LookupDictionary::Get(const SourceStr& source)
        {
            return _translations[_currentLocaleString][source];
        }
        //--------------------------------------------------------------------------

        const TranslationStr& LookupDictionary::Get(const SourceStr& source, const ContextStr& context)
        {
            return _translationsWithContext[_currentLocaleString][{source, context}];
        }
        //--------------------------------------------------------------------------
    }
}