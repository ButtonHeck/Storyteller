#include "config.h"

#include <rapidjson/pointer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

#include <fstream>

namespace Storyteller
{
#define JSON_KEY_CONFIG_LOG "Log"
#define JSON_KEY_CONFIG_LOG_FILENAME "Filename"
#define JSON_KEY_CONFIG_LOG_ENABLED "Enabled"
#define JSON_KEY_CONFIG_LOG_TRUNCATE "Truncate"
#define JSON_KEY_CONFIG_LOG_OUTPUT_CONSOLE "OutputConsole"
#define JSON_KEY_CONFIG_LOG_OUTPUT_FILE "OutputFile"
#define JSON_KEY_CONFIG_LOG_OUTPUT_STRING_BUFFER "OutputStringBuffer"
#define JSON_KEY_CONFIG_LOG_CORE_LEVEL "CoreLevel"
#define JSON_KEY_CONFIG_LOG_CLIENT_LEVEL "ClientLevel"

    Config::Config()
        : _document()
    {}
    //--------------------------------------------------------------------------

    bool Config::Load(const std::filesystem::path& path)
    {
        std::ifstream inputStream(path);
        if (!inputStream.is_open() || !inputStream.good())
        {
            return false;
        }

        rapidjson::IStreamWrapper jsonStream(inputStream);
        _document.ParseStream(jsonStream);
        inputStream.close();

        if (_document.HasParseError())
        {
            return false;
        }

        LoadLogConfig();

        return true;
    }
    //--------------------------------------------------------------------------

    const LogConfig& Config::GetLogConfig() const
    {
        return _log;
    }
    //--------------------------------------------------------------------------

    void Config::LoadLogConfig()
    {
        if (_document.HasMember(JSON_KEY_CONFIG_LOG) && _document[JSON_KEY_CONFIG_LOG].IsObject())
        {
            const auto logConfigJson = _document[JSON_KEY_CONFIG_LOG].GetObject();

            if (logConfigJson.HasMember(JSON_KEY_CONFIG_LOG_FILENAME) && logConfigJson[JSON_KEY_CONFIG_LOG_FILENAME].IsString())
            {
                _log.filename = logConfigJson[JSON_KEY_CONFIG_LOG_FILENAME].GetString();
            }

            if (logConfigJson.HasMember(JSON_KEY_CONFIG_LOG_ENABLED) && logConfigJson[JSON_KEY_CONFIG_LOG_ENABLED].IsBool())
            {
                _log.enabled = logConfigJson[JSON_KEY_CONFIG_LOG_ENABLED].GetBool();
            }

            if (logConfigJson.HasMember(JSON_KEY_CONFIG_LOG_TRUNCATE) && logConfigJson[JSON_KEY_CONFIG_LOG_TRUNCATE].IsBool())
            {
                _log.truncate = logConfigJson[JSON_KEY_CONFIG_LOG_TRUNCATE].GetBool();
            }

            if (logConfigJson.HasMember(JSON_KEY_CONFIG_LOG_OUTPUT_CONSOLE) && logConfigJson[JSON_KEY_CONFIG_LOG_OUTPUT_CONSOLE].IsBool())
            {
                _log.outputConsole = logConfigJson[JSON_KEY_CONFIG_LOG_OUTPUT_CONSOLE].GetBool();
            }

            if (logConfigJson.HasMember(JSON_KEY_CONFIG_LOG_OUTPUT_FILE) && logConfigJson[JSON_KEY_CONFIG_LOG_OUTPUT_FILE].IsBool())
            {
                _log.outputFile = logConfigJson[JSON_KEY_CONFIG_LOG_OUTPUT_FILE].GetBool();
            }

            if (logConfigJson.HasMember(JSON_KEY_CONFIG_LOG_OUTPUT_STRING_BUFFER) && logConfigJson[JSON_KEY_CONFIG_LOG_OUTPUT_STRING_BUFFER].IsBool())
            {
                _log.outputStringBuffer = logConfigJson[JSON_KEY_CONFIG_LOG_OUTPUT_STRING_BUFFER].GetBool();
            }

            if (logConfigJson.HasMember(JSON_KEY_CONFIG_LOG_CORE_LEVEL) && logConfigJson[JSON_KEY_CONFIG_LOG_CORE_LEVEL].IsInt())
            {
                _log.coreLevel = logConfigJson[JSON_KEY_CONFIG_LOG_CORE_LEVEL].GetInt();
            }

            if (logConfigJson.HasMember(JSON_KEY_CONFIG_LOG_CLIENT_LEVEL) && logConfigJson[JSON_KEY_CONFIG_LOG_CLIENT_LEVEL].IsInt())
            {
                _log.clientLevel = logConfigJson[JSON_KEY_CONFIG_LOG_CLIENT_LEVEL].GetInt();
            }
        }
    }
    //--------------------------------------------------------------------------
}