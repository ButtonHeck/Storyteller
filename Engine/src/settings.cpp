#include "settings.h"
#include "filesystem_utils.h"
#include "log.h"

#include <rapidjson/istreamwrapper.h>

#include <fstream>
#include <filesystem>

namespace Storyteller
{
    Settings::Settings(const std::string& name)
        : _name(name)
        , _filename(std::filesystem::current_path().append(name + ".json").generic_string())
        , _stringBuffer()
        , _writer(_stringBuffer)
        , _loadedDocument()
        , _scope("")
        , _success(true)
    {}
    //--------------------------------------------------------------------------

    bool Settings::StartSave()
    {
        STRTLR_CORE_LOG_INFO("Settings: saving to '{}'", _filename);

        if (!Filesystem::CheckPathAndTryCreate(_filename))
        {
            STRTLR_CORE_LOG_WARN("Settings: insufficient path");
            _success = false;
            return false;
        }

        _success &= _writer.StartObject();

        return _success;
    }
    //--------------------------------------------------------------------------

    bool Settings::EndSave()
    {
        _success &= _writer.EndObject();

        std::ofstream outputStream(_filename, std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            STRTLR_CORE_LOG_WARN("Settings: failed to open file stream");
            _success = false;
            return false;
        }

        outputStream << _stringBuffer.GetString();
        outputStream.close();

        return _success;
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveGroup(const std::string& groupName)
    {
        _success &= _writer.Key(groupName.c_str());
        _success &= _writer.StartObject();

        return _success;
    }
    //--------------------------------------------------------------------------    

    bool Settings::EndSaveGroup()
    {
        _success &= _writer.EndObject();

        return _success;
    }
    //--------------------------------------------------------------------------   

    bool Settings::SaveBool(const std::string& name, bool value)
    {
        _success &= _writer.Key(name.c_str());
        _success &= _writer.Bool(value);

        return _success;
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt(const std::string& name, int value)
    {
        _success &= _writer.Key(name.c_str());
        _success &= _writer.Int(value);

        return _success;
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt(const std::string& name, unsigned int value)
    {
        _success &= _writer.Key(name.c_str());
        _success &= _writer.Uint(value);

        return _success;
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt64(const std::string& name, int64_t value)
    {
        _success &= _writer.Key(name.c_str());
        _success &= _writer.Int64(value);

        return _success;
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt64(const std::string& name, uint64_t value)
    {
        _success &= _writer.Key(name.c_str());
        _success &= _writer.Uint64(value);

        return _success;
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveString(const std::string& name, const std::string& value)
    {
        _success &= _writer.Key(name.c_str());
        _success &= _writer.String(value.c_str());

        return _success;
    }
    //--------------------------------------------------------------------------

    bool Settings::StartLoad()
    {
        STRTLR_CORE_LOG_INFO("Settings: loading from '{}'", _filename);

        if (!std::filesystem::exists(_filename))
        {
            STRTLR_CORE_LOG_WARN("Settings: insufficient path");
            _success = false;
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
            _success = false;
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool Settings::EndLoad()
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool Settings::StartLoadGroup(const std::string& groupName)
    {
        if (!_loadedDocument.HasMember(groupName.c_str()) || !_loadedDocument[groupName.c_str()].IsObject())
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot find group '{}', or the group is not an object type", groupName);
            _success = false;
            return false;
        }

        _scope = groupName;

        return true;
    }

    bool Settings::EndLoadGroup()
    {
        _scope = "";

        return true;
    }
    //--------------------------------------------------------------------------

    bool Settings::GetBool(const std::string& name, bool defaultValue)
    {
        const auto scopeObject = _loadedDocument[_scope.c_str()].GetObject();
        if (!scopeObject.HasMember(name.c_str()))
        {
            STRTLR_CORE_LOG_ERROR("Settings: cannot find bool for '{}'", name);
            return defaultValue;
        }

        return scopeObject[name.c_str()].GetBool();
    }
    //--------------------------------------------------------------------------

    int Settings::GetInt(const std::string& name, int defaultValue)
    {
        return 0;
    }
    //--------------------------------------------------------------------------

    unsigned int Settings::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        return 0;
    }
    //--------------------------------------------------------------------------

    int64_t Settings::GetInt64(const std::string& name, int64_t defaultValue)
    {
        return 0;
    }
    //--------------------------------------------------------------------------

    uint64_t Settings::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        return 0;
    }
    //--------------------------------------------------------------------------

    std::string Settings::GetString(const std::string& name, const std::string& defaultValue)
    {
        return std::string();
    }
    //--------------------------------------------------------------------------     
}