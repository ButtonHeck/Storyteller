#include "settings_json_impl_writer.h"
#include "filesystem_utils.h"
#include "log.h"

#include <fstream>

namespace Storyteller
{
    SettingsJsonWriter::SettingsJsonWriter(const std::string& name)
        : _name(name)
        , _filename(Filesystem::GetCurrentPath().append(name + ".json").generic_string())
        , _stringBuffer()
        , _writer(_stringBuffer)
    {}
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::StartSave()
    {
        STRTLR_CORE_LOG_INFO("SettingsWriter: saving to '{}'", _filename);

        if (!Filesystem::CreatePathTree(_filename))
        {
            STRTLR_CORE_LOG_WARN("SettingsWriter: insufficient path");
            return false;
        }

        return _writer.StartObject();
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::EndSave()
    {
        _writer.EndObject();

        std::ofstream outputStream(_filename, std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            STRTLR_CORE_LOG_WARN("SettingsWriter: failed to open file stream");
            return false;
        }

        outputStream << _stringBuffer.GetString();
        outputStream.close();

        return true;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::StartSaveGroup(const std::string& groupName)
    {
        if (groupName.empty())
        {
            STRTLR_CORE_LOG_WARN("SettingsWriter: group name should not be empty!");
            return false;
        }

        if (_writer.Key(groupName.c_str()))
        {
            return _writer.StartObject();
        }

        STRTLR_CORE_LOG_WARN("SettingsWriter: cannot start saving group '{}'", groupName);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::EndSaveGroup()
    {
        return _writer.EndObject();
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::StartSaveArray(const std::string& arrayName)
    {
        if (_writer.Key(arrayName.c_str()))
        {
            return _writer.StartArray();
        }

        STRTLR_CORE_LOG_WARN("SettingsWriter: cannot save array '{}'", arrayName);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::EndSaveArray()
    {
        return _writer.EndArray();
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveBool(bool value)
    {
        return _writer.Bool(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveBool(const std::string& name, bool value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Bool(value);
        }

        STRTLR_CORE_LOG_WARN("SettingsWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveInt(int value)
    {
        return _writer.Int(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveInt(const std::string& name, int value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Int(value);
        }

        STRTLR_CORE_LOG_WARN("SettingsWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveUInt(unsigned int value)
    {
        return _writer.Uint(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveUInt(const std::string& name, unsigned int value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Uint(value);
        }

        STRTLR_CORE_LOG_WARN("SettingsWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveInt64(int64_t value)
    {
        return _writer.Int64(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveInt64(const std::string& name, int64_t value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Int64(value);
        }

        STRTLR_CORE_LOG_WARN("SettingsWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveUInt64(uint64_t value)
    {
        return _writer.Uint64(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveUInt64(const std::string& name, uint64_t value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Uint64(value);
        }

        STRTLR_CORE_LOG_WARN("SettingsWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveDouble(double value)
    {
        return _writer.Double(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveDouble(const std::string& name, double value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Double(value);
        }

        STRTLR_CORE_LOG_WARN("SettingsWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveString(const std::string& value)
    {
        return _writer.String(value.c_str());
    }
    //--------------------------------------------------------------------------

    bool SettingsJsonWriter::SaveString(const std::string& name, const std::string& value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.String(value.c_str());
        }

        STRTLR_CORE_LOG_WARN("SettingsWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------
}