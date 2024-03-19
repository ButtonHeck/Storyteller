#pragma once

#include "pointers.h"

#include <string>
#include <unordered_map>

namespace Storyteller
{
    class LocalizationLookupDictionary;

    class LocalizationLibrary
    {
    public:
        explicit LocalizationLibrary(const std::string& defaultLocale = "");

        void SetLocale(const std::string& localeString);

        Ptr<LocalizationLookupDictionary> AddLookupDictionary(const std::string& domain);
        Ptr<LocalizationLookupDictionary> GetLookupDictionary(const std::string& domain) const;

        void Add(const std::string& domain, const std::string& source, const std::string& translation);
        void Add(const std::string& domain, const std::string& source, const std::string& context, const std::string& translation);
        const std::string& Get(const std::string& domain, const std::string& source);
        const std::string& Get(const std::string& domain, const std::string& source, const std::string& context);

    private:
        typedef std::string DomainName;
        std::unordered_map<DomainName, Ptr<LocalizationLookupDictionary>> _lookupDictionaries;
        std::string _currentLocaleString;
    };
    //--------------------------------------------------------------------------
}