#include "localization_library.h"
#include "localization_dictionary.h"

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
    }
    //--------------------------------------------------------------------------

    Ptr<LocalizationDictionary> LocalizationLibrary::AddDictionary(const std::string& domain)
    {
        auto dictionary = CreatePtr<LocalizationDictionary>(domain);
        _dictionaries.insert(std::make_pair(domain, dictionary));
        return dictionary;
    }
    //--------------------------------------------------------------------------

    Ptr<LocalizationDictionary> LocalizationLibrary::GetDictionary(const std::string& domain) const
    {
        if (_dictionaries.contains(domain))
        {
            return _dictionaries.at(domain);
        }

        return nullptr;
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::RemoveDictionary(const std::string& domain)
    {
        _dictionaries.erase(domain);
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::Add(const std::string& domain, const std::string& source, const std::string& translation)
    {
        if (_dictionaries.contains(domain))
        {
            _dictionaries.at(domain)->Add(source, translation);
        }
    }
    //--------------------------------------------------------------------------

    void LocalizationLibrary::Add(const std::string& domain, const std::string& source, const std::string& context, const std::string& translation)
    {
        if (_dictionaries.contains(domain))
        {
            _dictionaries.at(domain)->Add(source, context, translation);
        }
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationLibrary::Get(const std::string& domain, const std::string& source)
    {
        if (_dictionaries.contains(domain))
        {
            return _dictionaries.at(domain)->Get(source);
        }

        return noTranslation;
    }
    //--------------------------------------------------------------------------

    const std::string& LocalizationLibrary::Get(const std::string& domain, const std::string& source, const std::string& context)
    {
        if (_dictionaries.contains(domain))
        {
            return _dictionaries.at(domain)->Get(source, context);
        }
        
        return noTranslation;
    }
    //--------------------------------------------------------------------------
}
