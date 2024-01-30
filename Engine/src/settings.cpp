#include "settings.h"
#include "filesystem_utils.h"
#include "log.h"

#include <fstream>

namespace Storyteller
{
    Settings::Settings(const std::string& name)
        : _filename(name + ".json")
        , _stringBuffer()
        , _writer(_stringBuffer)
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

        _success = true;

        return true;
    }
    //--------------------------------------------------------------------------

    bool Settings::EndSave()
    {
        std::ofstream outputStream(_filename, std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            STRTLR_CORE_LOG_WARN("Settings: failed to open file stream");
            _success = false;
            return false;
        }

        outputStream << _stringBuffer.GetString();
        outputStream.close();

        _success &= true;

        return _success;
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveGroup(const std::string& groupName)
    {
        _success &= _writer.Key(groupName.c_str());
        _success &= _writer.StartArray();

        return _success;
    }
    //--------------------------------------------------------------------------    

    bool Settings::EndSaveGroup()
    {
        _success &= _writer.EndArray();

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

    void Settings::StartLoad()
    {
    }
    //--------------------------------------------------------------------------

    void Settings::EndLoad()
    {
    }
    //--------------------------------------------------------------------------     
}