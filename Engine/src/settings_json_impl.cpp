#include "settings_json_impl.h"
#include "filesystem.h"

namespace Storyteller
{
    SettingsJson::SettingsJson(const std::string& name)
        : _filename(Filesystem::ToU8String(Filesystem::GetCurrentPath().append(name + ".json")))
        , _reader(new SettingsJsonReader(_filename))
        , _writer(new SettingsJsonWriter(_filename))
    {}
    //--------------------------------------------------------------------------

    bool SettingsJson::StartSave()
    {
        return _writer->StartSave();
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::EndSave()
    {
        return _writer->EndSave();
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::StartSaveGroup(const std::string& groupName)
    {
        return _writer->StartSaveGroup(groupName);
    }
    //--------------------------------------------------------------------------    

    bool SettingsJson::EndSaveGroup()
    {
        return _writer->EndSaveGroup();
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::StartSaveArray(const std::string& arrayName)
    {
        return _writer->StartSaveArray(arrayName);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::EndSaveArray()
    {
        return _writer->EndSaveArray();
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveBool(bool value)
    {
        return _writer->SaveBool(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveBool(const std::string& name, bool value)
    {
        return _writer->SaveBool(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveInt(int value)
    {
        return _writer->SaveInt(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveInt(const std::string& name, int value)
    {
        return _writer->SaveInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveUInt(unsigned int value)
    {
        return _writer->SaveUInt(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveUInt(const std::string& name, unsigned int value)
    {
        return _writer->SaveUInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveInt64(int64_t value)
    {
        return _writer->SaveInt64(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveInt64(const std::string& name, int64_t value)
    {
        return _writer->SaveInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveUInt64(uint64_t value)
    {
        return _writer->SaveUInt64(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveUInt64(const std::string& name, uint64_t value)
    {
        return _writer->SaveUInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveDouble(double value)
    {
        return _writer->SaveDouble(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveDouble(const std::string& name, double value)
    {
        return _writer->SaveDouble(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveString(const std::string& value)
    {
        return _writer->SaveString(value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::SaveString(const std::string& name, const std::string& value)
    {
        return _writer->SaveString(name, value);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::StartLoad()
    {
        return _reader->StartLoad();
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::EndLoad()
    {
        return _reader->EndLoad();
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::StartLoadGroup(const std::string& groupName)
    {
        return _reader->StartLoadGroup(groupName);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::EndLoadGroup()
    {
        return _reader->EndLoadGroup();
    }
    //--------------------------------------------------------------------------

    int SettingsJson::StartLoadArray(const std::string& arrayName)
    {
        return _reader->StartLoadArray(arrayName);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::EndLoadArray()
    {
        return _reader->EndLoadArray();
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::GetBool(int index, bool defaultValue)
    {
        return _reader->GetBool(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    bool SettingsJson::GetBool(const std::string& name, bool defaultValue)
    {
        return _reader->GetBool(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int SettingsJson::GetInt(int index, int defaultValue)
    {
        return _reader->GetInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int SettingsJson::GetInt(const std::string& name, int defaultValue)
    {
        return _reader->GetInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int SettingsJson::GetUInt(int index, unsigned int defaultValue)
    {
        return _reader->GetUInt(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int SettingsJson::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        return _reader->GetUInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t SettingsJson::GetInt64(int index, int64_t defaultValue)
    {
        return _reader->GetInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t SettingsJson::GetInt64(const std::string& name, int64_t defaultValue)
    {
        return _reader->GetInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t SettingsJson::GetUInt64(int index, uint64_t defaultValue)
    {
        return _reader->GetUInt64(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t SettingsJson::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        return _reader->GetUInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    double SettingsJson::GetDouble(int index, double defaultValue)
    {
        return _reader->GetDouble(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    double SettingsJson::GetDouble(const std::string& name, double defaultValue)
    {
        return _reader->GetDouble(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    std::string SettingsJson::GetString(int index, const std::string& defaultValue)
    {
        return _reader->GetString(index, defaultValue);
    }
    //--------------------------------------------------------------------------

    std::string SettingsJson::GetString(const std::string& name, const std::string& defaultValue)
    {
        return _reader->GetString(name, defaultValue);
    }
    //--------------------------------------------------------------------------     
}