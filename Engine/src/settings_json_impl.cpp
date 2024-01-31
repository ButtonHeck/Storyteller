#include "settings_json_impl.h"
#include "filesystem_utils.h"
#include "log.h"

#include <rapidjson/istreamwrapper.h>

#include <algorithm>
#include <numeric>
#include <fstream>
#include <filesystem>

namespace Storyteller
{
    SettingsJson::SettingsJson(const std::string& name)
        : _name(name)
        , _filename(std::filesystem::current_path().append(name + ".json").generic_string())
        , _stringBuffer()
        , _writer(_stringBuffer)
        , _loadedDocument()
        , _scope()
    {}
    //--------------------------------------------------------------------------

    bool SettingsJson::StartSave()
    {
        STRTLR_CORE_LOG_INFO("Settings: saving to '{}'", _filename);

        if (!Filesystem::CheckPathAndTryCreate(_filename))
        {
            STRTLR_CORE_LOG_WARN("Settings: insufficient path");
            return false;
        }

        return _writer.StartObject();
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::EndSave()
    {
        _writer.EndObject();

        std::ofstream outputStream(_filename, std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            STRTLR_CORE_LOG_WARN("Settings: failed to open file stream");
            return false;
        }

        outputStream << _stringBuffer.GetString();
        outputStream.close();

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::StartSaveGroup(const std::string& groupName)
    {
        if (groupName.empty())
        {
            STRTLR_CORE_LOG_WARN("Settings: group name should not be empty!");
            return false;
        }

        if (_writer.Key(groupName.c_str()))
        {
            return _writer.StartObject();
        }

        STRTLR_CORE_LOG_WARN("Settings: cannot start saving group '{}'", groupName);
        return false;
    }
    //--------------------------------------------------------------------------    

    bool SettingsJson::EndSaveGroup()
    {
        return _writer.EndObject();
    }
    //--------------------------------------------------------------------------   

    bool SettingsJson::SaveBool(const std::string& name, bool value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Bool(value);
        }
        
        STRTLR_CORE_LOG_WARN("Settings: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveInt(const std::string& name, int value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Int(value);
        }

        STRTLR_CORE_LOG_WARN("Settings: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveUInt(const std::string& name, unsigned int value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Uint(value);
        }

        STRTLR_CORE_LOG_WARN("Settings: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveInt64(const std::string& name, int64_t value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Int64(value);
        }

        STRTLR_CORE_LOG_WARN("Settings: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveUInt64(const std::string& name, uint64_t value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Uint64(value);
        }

        STRTLR_CORE_LOG_WARN("Settings: cannot save '{}'", name);
        return false;
    }

    bool SettingsJson::SaveDouble(const std::string& name, double value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Double(value);
        }

        STRTLR_CORE_LOG_WARN("Settings: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveString(const std::string& name, const std::string& value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.String(value.c_str());
        }

        STRTLR_CORE_LOG_WARN("Settings: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::StartLoad()
    {
        STRTLR_CORE_LOG_INFO("Settings: loading from '{}'", _filename);

        if (!std::filesystem::exists(_filename))
        {
            STRTLR_CORE_LOG_WARN("Settings: insufficient path");
            return false;
        }

        std::ifstream inputStream(_filename);
        if (!inputStream.is_open() || !inputStream.good())
        {
            return false;
        }

        rapidjson::IStreamWrapper jsonStream(inputStream);
        _loadedDocument.ParseStream(jsonStream);
        inputStream.close();

        if (_loadedDocument.HasParseError())
        {
            STRTLR_CORE_LOG_ERROR("Settings: JSON parsing error '{}'", _loadedDocument.GetParseError());
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::EndLoad()
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::StartLoadGroup(const std::string& groupName)
    {
        if (!_loadedDocument.IsObject())
        {
            return false;
        }

        if (groupName.empty())
        {
            STRTLR_CORE_LOG_WARN("Settings: group name should not be empty!");
            return false;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot fetch object pointer for '{}'", currentScopeString);
            return false;
        }

        if (!currentScopeObjectPointer->HasMember(groupName.c_str()) || !(*currentScopeObjectPointer)[groupName.c_str()].IsObject())
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot find group '{}', or the group is not an object type", groupName);
            return false;
        }

        _scope.push_back(groupName);

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::EndLoadGroup()
    {
        if (!_scope.empty())
        {
            _scope.pop_back();
            return true;
        }

        STRTLR_CORE_LOG_ERROR("Settings: cannot end load group, already at the root");
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::GetBool(const std::string& name, bool defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsBool())
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot find bool for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetBool();
    }
    //--------------------------------------------------------------------------

    int SettingsJson::GetInt(const std::string& name, int defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsInt())
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot find int for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetInt();
    }
    //--------------------------------------------------------------------------

    unsigned int SettingsJson::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsUint())
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot find uint for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetUint();
    }
    //--------------------------------------------------------------------------

    int64_t SettingsJson::GetInt64(const std::string& name, int64_t defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsInt64())
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot find int64 for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetInt64();
    }
    //--------------------------------------------------------------------------

    uint64_t SettingsJson::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsUint64())
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot find uint64 for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetUint64();
    }
    //--------------------------------------------------------------------------

    double SettingsJson::GetDouble(const std::string& name, double defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsDouble())
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot find double for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetDouble();
    }
    //--------------------------------------------------------------------------

    std::string SettingsJson::GetString(const std::string& name, const std::string& defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsString())
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot find string for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetString();
    }
    //--------------------------------------------------------------------------     

    std::string SettingsJson::GetCurrentScopeString() const
    {
        return _scope.empty()
            ? ""
            : std::accumulate(_scope.begin(), _scope.end(), std::string(), 
                [](const std::string& a, const std::string& b) {
                    return a + '/' + b;
                });
    }
    //--------------------------------------------------------------------------     
}