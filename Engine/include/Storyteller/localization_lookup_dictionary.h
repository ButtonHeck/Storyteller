#pragma once

#include <string>
#include <unordered_map>

namespace Storyteller
{
    class LocalizationLookupDictionary
    {
    public:
        explicit LocalizationLookupDictionary(const std::string& domain, const std::string& defaultLocale = "");

        const std::string& GetDomain() const;
        void SetLocale(const std::string& locale);

        void Add(const std::string& source, const std::string& translation);
        void Add(const std::string& source, const std::string& context, const std::string& translation);
        const std::string& Get(const std::string& source);
        const std::string& Get(const std::string& source, const std::string& context);

    private:
        typedef std::string LocaleName;
        typedef std::string SourceString;
        typedef std::string ContextString;
        typedef std::string TranslationString;

        struct ContextedSource
        {
            SourceString source;
            ContextString context;

            bool operator==(const ContextedSource& other) const
            {
                return source == other.source && context == other.context;
            }
        };

        struct ContextedSourceHash
        {
            std::size_t operator()(const ContextedSource& p) const
            {
                const auto hs = std::hash<SourceString>{}(p.source);
                const auto hc = std::hash<ContextString>{}(p.context);

                return hs ^ hc;
            }
        };

        typedef std::unordered_map<SourceString, TranslationString> Translations;
        typedef std::unordered_map<ContextedSource, TranslationString, ContextedSourceHash> ContextedTranslations;
        typedef std::unordered_map<LocaleName, Translations> LocalizedTranslations;
        typedef std::unordered_map<LocaleName, ContextedTranslations> LocalizedContextedTranslations;

    private:
        const std::string _domain;
        LocaleName _currentLocaleString;
        LocalizedTranslations _translations;
        LocalizedContextedTranslations _translationsWithContext;
    };
    //--------------------------------------------------------------------------
}