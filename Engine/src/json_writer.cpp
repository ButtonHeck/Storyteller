#include "json_writer.h"
#include "log.h"

#include <fstream>

namespace Storyteller
{
    JsonWriter::JsonWriter(const std::filesystem::path& path)
        : _path(path)
        , _stringBuffer()
        , _writer(_stringBuffer)
    {}
    //--------------------------------------------------------------------------

    bool JsonWriter::Start()
    {
        STRTLR_CORE_LOG_INFO("JsonWriter: saving to '{}'", Filesystem::ToU8String(_path));

        if (!Filesystem::CreatePathTree(_path))
        {
            STRTLR_CORE_LOG_WARN("JsonWriter: insufficient path");
            return false;
        }

        return _writer.StartObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::End()
    {
        _writer.EndObject();

        std::ofstream outputStream(_path, std::ios::out | std::ios::trunc);
        if (!outputStream.is_open() || !outputStream.good())
        {
            STRTLR_CORE_LOG_WARN("JsonWriter: failed to open file stream");
            return false;
        }

        outputStream << _stringBuffer.GetString();
        outputStream.close();

        return true;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartObject()
    {
        return _writer.StartObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndObject()
    {
        return _writer.EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartGroup(const std::string& groupName)
    {
        if (groupName.empty())
        {
            STRTLR_CORE_LOG_WARN("JsonWriter: group name should not be empty!");
            return false;
        }

        if (_writer.Key(groupName.c_str()))
        {
            return _writer.StartObject();
        }

        STRTLR_CORE_LOG_WARN("JsonWriter: cannot start saving group '{}'", groupName);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndGroup()
    {
        return _writer.EndObject();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::StartArray(const std::string& arrayName)
    {
        if (_writer.Key(arrayName.c_str()))
        {
            return _writer.StartArray();
        }

        STRTLR_CORE_LOG_WARN("JsonWriter: cannot save array '{}'", arrayName);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::EndArray()
    {
        return _writer.EndArray();
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveBool(bool value)
    {
        return _writer.Bool(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveBool(const std::string& name, bool value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Bool(value);
        }

        STRTLR_CORE_LOG_WARN("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt(int value)
    {
        return _writer.Int(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt(const std::string& name, int value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Int(value);
        }

        STRTLR_CORE_LOG_WARN("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt(unsigned int value)
    {
        return _writer.Uint(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt(const std::string& name, unsigned int value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Uint(value);
        }

        STRTLR_CORE_LOG_WARN("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt64(int64_t value)
    {
        return _writer.Int64(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveInt64(const std::string& name, int64_t value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Int64(value);
        }

        STRTLR_CORE_LOG_WARN("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt64(uint64_t value)
    {
        return _writer.Uint64(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveUInt64(const std::string& name, uint64_t value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Uint64(value);
        }

        STRTLR_CORE_LOG_WARN("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveDouble(double value)
    {
        return _writer.Double(value);
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveDouble(const std::string& name, double value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.Double(value);
        }

        STRTLR_CORE_LOG_WARN("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveString(const std::string& value)
    {
        return _writer.String(value.c_str());
    }
    //--------------------------------------------------------------------------

    bool JsonWriter::SaveString(const std::string& name, const std::string& value)
    {
        if (_writer.Key(name.c_str()))
        {
            return _writer.String(value.c_str());
        }

        STRTLR_CORE_LOG_WARN("JsonWriter: cannot save '{}'", name);
        return false;
    }
    //--------------------------------------------------------------------------
}