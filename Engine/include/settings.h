#pragma once 

#include <string>
#include <memory>

namespace Storyteller
{
    class SettingsJson;

    class Settings
    {
    public:
        typedef std::shared_ptr<Settings> Ptr;

        explicit Settings(const std::string& name);

        bool StartSave();
        bool EndSave();
        bool StartSaveGroup(const std::string& groupName);
        bool EndSaveGroup();
        bool SaveBool(const std::string& name, bool value);
        bool SaveInt(const std::string& name, int value);
        bool SaveUInt(const std::string& name, unsigned int value);
        bool SaveInt64(const std::string& name, int64_t value);
        bool SaveUInt64(const std::string& name, uint64_t value);
        bool SaveDouble(const std::string& name, double value);
        bool SaveString(const std::string& name, const std::string& value);

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
        std::shared_ptr<SettingsJson> _impl;
    };
    //--------------------------------------------------------------------------
}