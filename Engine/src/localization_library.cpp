#include "localization_library.h"
#include "localization_lookup_dictionary.h"

namespace Storyteller
{
    static const std::string noTranslation = std::string("");

    LocalizationLibrary::LocalizationLibrary(const std::string& defaultLocale)
        : _currentLocaleString(defaultLocale)
    {}
    //--------------------------------------------------------------------------

    void LocalizationLibrary::SetLocale(const std::string& localeString)
    {
        _currentLocaleString = localeString;
        for (auto& dict : _lookupDictionaries)
        {
            dict.second->SetLocale(localeString);
        }
    }
    //--------------------------------------------------------------------------

    Ptr<LocalizationLookupDictionary> LocalizationLibrary::AddLookupDictionary(const std::string& domain)
    {
        auto dictionary = CreatePtr<LocalizationLookupDictionary>(domain, _currentLocaleString);
        _lookupDictionaries.insert(std::make_pair(domain, dictionary));
        return dictionary;
    }
    //--------------------------------------------------------------------------

    Ptr<LocalizationLookupDictionary> LocalizationLibrary::GetLookupDictionary(const std::string& domain) const
    {
        if (_lookupDictionaries.contains(domain))
        {
            return _lookupDictionaries.at(domain);
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::Add(const std::string& domain, const std::string& source, const std::string& translation)
    {
        if (_lookupDictionaries.contains(domain))
        {
            _lookupDictionaries.at(domain)->Add(source, translation);
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::Add(const std::string& domain, const std::string& source, const std::string& context, const std::string& translation)
    {
        if (_lookupDictionaries.contains(domain))
        {
            _lookupDictionaries.at(domain)->Add(source, context, translation);
        }
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationLibrary::Get(const std::string& domain, const std::string& source)
    {
        if (_lookupDictionaries.contains(domain))
        {
            return _lookupDictionaries.at(domain)->Get(source);
        }

        return noTranslation;
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationLibrary::Get(const std::string& domain, const std::string& source, const std::string& context)
    {
        if (_lookupDictionaries.contains(domain))
        {
            return _lookupDictionaries.at(domain)->Get(source, context);
        }
        
        return noTranslation;
    }
    //--------------------------------------------------------------------------
}
