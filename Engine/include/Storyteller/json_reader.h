#pragma once

#include <rapidjson/document.h>

#include <string>
#include <vector>

namespace Storyteller
{
    class JsonReader
    {
    public:
        explicit JsonReader(const std::string& filename);

        bool StartLoad();
        bool EndLoad();

        bool StartLoadArrayObject(int index);
        bool EndLoadArrayObject();

        bool StartLoadGroup(const std::string& groupName);
        bool EndLoadGroup();

        int StartLoadArray(const std::string& arrayName);
        bool EndLoadArray();

        bool GetBool(int index, bool defaultValue = false);
        bool GetBool(const std::string& name, bool defaultValue = false);
        int GetInt(int index, int defaultValue = 0);
        int GetInt(const std::string& name, int defaultValue = 0);
        unsigned int GetUInt(int index, unsigned int defaultValue = 0);
        unsigned int GetUInt(const std::string& name, unsigned int defaultValue = 0);
        int64_t GetInt64(int index, int64_t defaultValue = 0);
        int64_t GetInt64(const std::string& name, int64_t defaultValue = 0);
        uint64_t GetUInt64(int index, uint64_t defaultValue = 0);
        uint64_t GetUInt64(const std::string& name, uint64_t defaultValue = 0);
        double GetDouble(int index, double defaultValue = 0.0);
        double GetDouble(const std::string& name, double defaultValue = 0.0);
        std::string GetString(int index, const std::string& defaultValue = "");
        std::string GetString(const std::string& name, const std::string& defaultValue = "");

    private:
        std::string GetCurrentScopeString() const;
        bool ValidToGetFromArray(int index) const;

    private:
        const std::string _filename;

        rapidjson::Document _document;
        std::vector<std::string> _scope;
        std::string _scopeString;
        rapidjson::Value* _currentObject;
    };
    //--------------------------------------------------------------------------
}