#include "settings_json_impl_reader.h"
#include "log.h"

#include <rapidjson/pointer.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

#include <algorithm>
#include <numeric>
#include <fstream>
#include <filesystem>

namespace Storyteller
{
    SettingsJsonReader::SettingsJsonReader(const std::string& name)
        : _name(name)
        , _filename(std::filesystem::current_path().append(name + ".json").generic_string())
        , _document()
        , _scope()
        , _scopeString("")
        , _currentObject(nullptr)
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
        _document.ParseStream(jsonStream);
        inputStream.close();

        if (_document.HasParseError())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: JSON parsing error '{}'", rapidjson::GetParseError_En(_document.GetParseError()));
            return false;
        }

        _currentObject = rapidjson::Pointer("").Get(_document);

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
        if (!_document.IsObject())
        {
            return false;
        }

        if (groupName.empty())
        {
            STRTLR_CORE_LOG_WARN("SettingsReader: group name should not be empty!");
            return false;
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);
        if (!_currentObject)
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot fetch object pointer for '{}'", _scopeString);
            return false;
        }

        if (!_currentObject->HasMember(groupName.c_str()) || !(*_currentObject)[groupName.c_str()].IsObject())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find group '{}', or the group is not an object type", groupName);
            return false;
        }

        _scope.push_back(groupName);
        _scopeString = GetCurrentScopeString();
        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonReader::EndLoadGroup()
    {
        if (!_scope.empty())
        {
            _scope.pop_back();
            _scopeString = GetCurrentScopeString();
            _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

            return true;
        }

        STRTLR_CORE_LOG_ERROR("SettingsReader: cannot end load group, already at the root");
        return false;
    }
    //--------------------------------------------------------------------------

    int SettingsJsonReader::StartLoadArray(const std::string& arrayName)
    {
        if (!_document.IsObject())
        {
            return 0;
        }

        if (arrayName.empty())
        {
            STRTLR_CORE_LOG_WARN("SettingsReader: group name should not be empty!");
            return 0;
        }

        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);
        if (!_currentObject)
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot fetch object pointer for '{}'", _scopeString);
            return 0;
        }

        if (!_currentObject->HasMember(arrayName.c_str()) || !(*_currentObject)[arrayName.c_str()].IsArray())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find group '{}', or the group is not an array type", arrayName);
            return 0;
        }

        _scope.push_back(arrayName);
        _scopeString = GetCurrentScopeString();
        _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

        return (*_currentObject).GetArray().Size();
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonReader::EndLoadArray()
    {
        if (!_scope.empty())
        {
            _scope.pop_back();
            _scopeString = GetCurrentScopeString();
            _currentObject = rapidjson::Pointer(_scopeString.c_str()).Get(_document);

            return true;
        }

        STRTLR_CORE_LOG_ERROR("SettingsReader: cannot end load group, already at the root");
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonReader::GetBool(int index, bool defaultValue)
    {
        // todo
        return defaultValue;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonReader::GetBool(const std::string& name, bool defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsBool())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find bool for '{}/{}'", _scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetBool();
    }
    //--------------------------------------------------------------------------

    int SettingsJsonReader::GetInt(int index, int defaultValue)
    {
        // todo
        return defaultValue;
    }
    //--------------------------------------------------------------------------

    int SettingsJsonReader::GetInt(const std::string& name, int defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find int for '{}/{}'", _scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetInt();
    }
    //--------------------------------------------------------------------------

    unsigned int SettingsJsonReader::GetUInt(int index, unsigned int defaultValue)
    {
        // todo
        return defaultValue;
    }
    //--------------------------------------------------------------------------

    unsigned int SettingsJsonReader::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find uint for '{}/{}'", _scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetUint();
    }
    //--------------------------------------------------------------------------

    int64_t SettingsJsonReader::GetInt64(int index, int64_t defaultValue)
    {
        // todo
        return defaultValue;
    }
    //--------------------------------------------------------------------------

    int64_t SettingsJsonReader::GetInt64(const std::string& name, int64_t defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsInt64())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find int64 for '{}/{}'", _scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetInt64();
    }
    //--------------------------------------------------------------------------

    uint64_t SettingsJsonReader::GetUInt64(int index, uint64_t defaultValue)
    {
        //todo
        return defaultValue;
    }
    //--------------------------------------------------------------------------

    uint64_t SettingsJsonReader::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsUint64())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find uint64 for '{}/{}'", _scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetUint64();
    }
    //--------------------------------------------------------------------------

    double SettingsJsonReader::GetDouble(int index, double defaultValue)
    {
        //todo
        return defaultValue;
    }
    //--------------------------------------------------------------------------

    double SettingsJsonReader::GetDouble(const std::string& name, double defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsDouble())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find double for '{}/{}'", _scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetDouble();
    }
    //--------------------------------------------------------------------------

    std::string SettingsJsonReader::GetString(int index, const std::string& defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!(*_currentObject)[index].IsString())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find string for '{}[{}]'", _scopeString, index);
            return defaultValue;
        }

        return (*_currentObject)[index].GetString();
    }
    //--------------------------------------------------------------------------

    std::string SettingsJsonReader::GetString(const std::string& name, const std::string& defaultValue)
    {
        if (!_currentObject)
        {
            return defaultValue;
        }

        if (!_currentObject->HasMember(name.c_str()) || !(*_currentObject)[name.c_str()].IsString())
        {
            STRTLR_CORE_LOG_ERROR("SettingsReader: cannot find string for '{}/{}'", _scopeString, name);
            return defaultValue;
        }

        return (*_currentObject)[name.c_str()].GetString();
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