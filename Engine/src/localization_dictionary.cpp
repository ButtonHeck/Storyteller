#include "localization_dictionary.h"

namespace Storyteller
{
    LocalizationDictionary::LocalizationDictionary(const std::string& domain)
        : _domain(domain)
    {}
    //--------------------------------------------------------------------------

    const std::string& LocalizationDictionary::GetDomain() const
    {
        return _domain;
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const std::string& source, const std::string& translation)
    {
        _translations.insert(std::make_pair(source, translation));
    }
    //--------------------------------------------------------------------------

    void LocalizationDictionary::Add(const std::string& source, const std::string& context, const std::string& translation)
    {
        _translationsWithContext.insert(std::make_pair(ContextedSource(source, context), translation));
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationDictionary::Get(const std::string& source)
    {
        return _translations[source];
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationDictionary::Get(const std::string& source, const std::string& context)
    {
        return _translationsWithContext[{source, context}];
    }
    //--------------------------------------------------------------------------
}