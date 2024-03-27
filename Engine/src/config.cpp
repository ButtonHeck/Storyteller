#include "config.h"

#include <rapidjson/pointer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

#include <fstream>

namespace Storyteller
{
#define JSON_KEY_CONFIG_LOG "Log"
#define JSON_KEY_CONFIG_LOG_NAME "Name"
#define JSON_KEY_CONFIG_LOG_TRUNCATE "Truncate"

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
            if (logConfigJson.HasMember(JSON_KEY_CONFIG_LOG_NAME) && logConfigJson[JSON_KEY_CONFIG_LOG_NAME].IsString())
            {
                _log.name = logConfigJson[JSON_KEY_CONFIG_LOG_NAME].GetString();
            }

            if (logConfigJson.HasMember(JSON_KEY_CONFIG_LOG_TRUNCATE) && logConfigJson[JSON_KEY_CONFIG_LOG_TRUNCATE].IsBool())
            {
                _log.truncate = logConfigJson[JSON_KEY_CONFIG_LOG_TRUNCATE].GetBool();
            }
        }
    }
    //--------------------------------------------------------------------------
}