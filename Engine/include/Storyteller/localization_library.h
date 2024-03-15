#pragma once

#include "pointers.h"

#include <string>
#include <unordered_map>

namespace Storyteller
{
    class LocalizationDictionary;

    class LocalizationLibrary
    {
    public:
        Ptr<LocalizationDictionary> AddDictionary(const std::string& domain);
        Ptr<LocalizationDictionary> GetDictionary(const std::string& domain) const;
        void RemoveDictionary(const std::string& domain);

        void Add(const std::string& domain, const std::string& source, const std::string& translation);
        void Add(const std::string& domain, const std::string& source, const std::string& context, const std::string& translation);
        const std::string& Get(const std::string& domain, const std::string& source);
        const std::string& Get(const std::string& domain, const std::string& source, const std::string& context);

    private:
        std::unordered_map<std::string, Ptr<LocalizationDictionary>> _dictionaries;
    };
    //--------------------------------------------------------------------------
}