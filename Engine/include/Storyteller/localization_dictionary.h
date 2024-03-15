#pragma once

#include <string>
#include <unordered_map>

namespace Storyteller
{
    class LocalizationDictionary
    {
    public:
        explicit LocalizationDictionary(const std::string& domain);

        const std::string& GetDomain() const;

        void Add(const std::string& source, const std::string& translation);
        void Add(const std::string& source, const std::string& context, const std::string& translation);
        const std::string& Get(const std::string& source);
        const std::string& Get(const std::string& source, const std::string& context);

    private:
        struct ContextedSource
        {
            std::string source;
            std::string context;

            bool operator==(const ContextedSource& other) const
            {
                return source == other.source && context == other.context;
            }
        };

        struct ContextedSourceHash
        {
            std::size_t operator()(const ContextedSource& p) const
            {
                const auto hs = std::hash<std::string>{}(p.source);
                const auto hc = std::hash<std::string>{}(p.context);

                return hs ^ hc;
            }
        };

    private:
        const std::string _domain;
        std::unordered_map<std::string, std::string> _translations;
        std::unordered_map<ContextedSource, std::string, ContextedSourceHash> _translationsWithContext;
    };
    //--------------------------------------------------------------------------
}