#pragma once

#include <rapidjson/document.h>

#include <string>
#include <memory>
#include <vector>

namespace Storyteller
{
    class SettingsJsonReader
    {
    public:
        typedef std::shared_ptr<SettingsJsonReader> Ptr;

        explicit SettingsJsonReader(const std::string& name);

        bool StartLoad();
        bool EndLoad();
        bool StartLoadGroup(const std::string& groupName);
        bool EndLoadGroup();

        bool GetBool(const std::string& name, bool defaultValue = false);
        int GetInt(const std::string& name, int defaultValue = 0);
        unsigned int GetUInt(const std::string& name, unsigned int defaultValue = 0);
        int64_t GetInt64(const std::string& name, int64_t defaultValue = 0);
        uint64_t GetUInt64(const std::string& name, uint64_t defaultValue = 0);
        double GetDouble(const std::string& name, double defaultValue = 0);
        std::string GetString(const std::string& name, const std::string& defaultValue = "");

    private:
        std::string GetCurrentScopeString() const;

    private:
        const std::string _name;
        const std::string _filename;
        rapidjson::Document _document;
        std::vector<std::string> _scope;
        std::string _scopeString;
        rapidjson::Value* _currentGroup;
    };
    //--------------------------------------------------------------------------
}