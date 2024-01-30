#pragma once 

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#include <string>
#include <memory>

namespace Storyteller
{
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
        std::string GetString(const std::string& name, const std::string& defaultValue = "");

    private:
        const std::string _name;
        const std::string _filename;
        rapidjson::StringBuffer _stringBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> _writer;
        rapidjson::Document _loadedDocument;
        std::string _scope; //TODO: stack
        bool _success;
    };
    //--------------------------------------------------------------------------
}