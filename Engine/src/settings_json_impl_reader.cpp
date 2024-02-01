#include "settings_json_impl_reader.h"
#include "log.h"

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/pointer.h>

#include <algorithm>
#include <numeric>
#include <fstream>
#include <filesystem>

namespace Storyteller
{
    SettingsJsonReader::SettingsJsonReader(const std::string& name)
        : _name(name)
        , _filename(std::filesystem::current_path().append(name + ".json").generic_string())
        , _loadedDocument()
        , _scope()
    {}
    //--------------------------------------------------------------------------

    bool SettingsJsonReader::StartLoad()
    {
        STRTLR_CORE_LOG_INFO("SettingsReader: loading from '{}'", _filename);

        if (!std::filesystem::exists(_filename))
        {
            STRTLR_CORE_LOG_WARN("SettingsReader: insufficient path");
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
            STRTLR_CORE_LOG_ERROR("SettingsReader: JSON parsing error '{}'", _loadedDocument.GetParseError());
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonReader::EndLoad()
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonReader::StartLoadGroup(const std::string& groupName)
    {
        if (!_loadedDocument.IsObject())
        {
            return false;
        }

        if (groupName.empty())
        {
            STRTLR_CORE_LOG_WARN("SettingsReader: group name should not be empty!");
            return false;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot fetch object pointer for '{}'", currentScopeString);
            return false;
        }

        if (!currentScopeObjectPointer->HasMember(groupName.c_str()) || !(*currentScopeObjectPointer)[groupName.c_str()].IsObject())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find group '{}', or the group is not an object type", groupName);
            return false;
        }

        _scope.push_back(groupName);

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonReader::EndLoadGroup()
    {
        if (!_scope.empty())
        {
            _scope.pop_back();
            return true;
        }

        STRTLR_CORE_LOG_ERROR("SettingsReader: cannot end load group, already at the root");
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonReader::GetBool(const std::string& name, bool defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsBool())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find bool for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetBool();
    }
    //--------------------------------------------------------------------------

    int SettingsJsonReader::GetInt(const std::string& name, int defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsInt())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find int for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetInt();
    }
    //--------------------------------------------------------------------------

    unsigned int SettingsJsonReader::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsUint())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find uint for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetUint();
    }
    //--------------------------------------------------------------------------

    int64_t SettingsJsonReader::GetInt64(const std::string& name, int64_t defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsInt64())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find int64 for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetInt64();
    }
    //--------------------------------------------------------------------------

    uint64_t SettingsJsonReader::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsUint64())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find uint64 for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetUint64();
    }
    //--------------------------------------------------------------------------

    double SettingsJsonReader::GetDouble(const std::string& name, double defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsDouble())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find double for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetDouble();
    }
    //--------------------------------------------------------------------------

    std::string SettingsJsonReader::GetString(const std::string& name, const std::string& defaultValue)
    {
        if (!_loadedDocument.IsObject())
        {
            return defaultValue;
        }

        const auto currentScopeString = GetCurrentScopeString();
        const auto currentScopeObjectPointer = rapidjson::Pointer(currentScopeString.c_str()).Get(_loadedDocument);
        if (!currentScopeObjectPointer)
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot fetch object pointer for '{}'", currentScopeString);
            return defaultValue;
        }

        if (!currentScopeObjectPointer->HasMember(name.c_str()) || !(*currentScopeObjectPointer)[name.c_str()].IsString())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find string for '{}/{}'", currentScopeString, name);
            return defaultValue;
        }

        return (*currentScopeObjectPointer)[name.c_str()].GetString();
    }
    //--------------------------------------------------------------------------

    std::string SettingsJsonReader::GetCurrentScopeString() const
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