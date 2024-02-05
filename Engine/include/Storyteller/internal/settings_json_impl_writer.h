#pragma once

#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <string>

namespace Storyteller
{
    class SettingsJsonWriter
    {
    public:
        explicit SettingsJsonWriter(const std::string& name);

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

    private:
        const std::string _name;
        const std::string _filename;

        rapidjson::StringBuffer _stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> _writer;
    };
    //--------------------------------------------------------------------------
}