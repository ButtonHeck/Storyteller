#include "localization_lookup_dictionary.h"

namespace Storyteller
{
    LocalizationLookupDictionary::LocalizationLookupDictionary(const std::string& domain, const std::string& defaultLocale)
        : _domain(domain)
        , _currentLocaleString(defaultLocale)
    {}
    //--------------------------------------------------------------------------

    const std::string& LocalizationLookupDictionary::GetDomain() const
    {
        return _domain;
    }
    //--------------------------------------------------------------------------

    void LocalizationLookupDictionary::SetLocale(const std::string& locale)
    {
        _currentLocaleString = locale;
    }
    //--------------------------------------------------------------------------

    void LocalizationLookupDictionary::Add(const std::string& source, const std::string& translation)
    {
        _translations[_currentLocaleString].insert(std::make_pair(source, translation));
    }
    //--------------------------------------------------------------------------

    void LocalizationLookupDictionary::Add(const std::string& source, const std::string& context, const std::string& translation)
    {
        _translationsWithContext[_currentLocaleString].insert(std::make_pair(ContextedSource(source, context), translation));
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationLookupDictionary::Get(const std::string& source)
    {
        return _translations[_currentLocaleString][source];
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationLookupDictionary::Get(const std::string& source, const std::string& context)
    {
        return _translationsWithContext[_currentLocaleString][{source, context}];
    }
    //--------------------------------------------------------------------------
}