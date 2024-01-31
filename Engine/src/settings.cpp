#include "settings.h"
#include "settings_json_impl.h"

namespace Storyteller
{
    Settings::Settings(const std::string& name)
        : _impl(new SettingsJson(name))
    {}
    //--------------------------------------------------------------------------

    bool Settings::StartSave()
    {
        return _impl->StartSave();
    }
    //--------------------------------------------------------------------------

    bool Settings::EndSave()
    {
        return _impl->EndSave();
    }
    //--------------------------------------------------------------------------

    bool Settings::StartSaveGroup(const std::string& groupName)
    {
        return _impl->StartSaveGroup(groupName);
    }
    //--------------------------------------------------------------------------    

    bool Settings::EndSaveGroup()
    {
        return _impl->EndSaveGroup();
    }
    //--------------------------------------------------------------------------   

    bool Settings::SaveBool(const std::string& name, bool value)
    {
        return _impl->SaveBool(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt(const std::string& name, int value)
    {
        return _impl->SaveInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt(const std::string& name, unsigned int value)
    {
        return _impl->SaveUInt(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveInt64(const std::string& name, int64_t value)
    {
        return _impl->SaveInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveUInt64(const std::string& name, uint64_t value)
    {
        return _impl->SaveUInt64(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveDouble(const std::string& name, double value)
    {
        return _impl->SaveDouble(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::SaveString(const std::string& name, const std::string& value)
    {
        return _impl->SaveString(name, value);
    }
    //--------------------------------------------------------------------------

    bool Settings::StartLoad()
    {
        return _impl->StartLoad();
    }
    //--------------------------------------------------------------------------

    bool Settings::EndLoad()
    {
        return _impl->EndLoad();
    }
    //--------------------------------------------------------------------------

    bool Settings::StartLoadGroup(const std::string& groupName)
    {
        return _impl->StartLoadGroup(groupName);
    }
    //--------------------------------------------------------------------------

    bool Settings::EndLoadGroup()
    {
        return _impl->EndLoadGroup();
    }
    //--------------------------------------------------------------------------

    bool Settings::GetBool(const std::string& name, bool defaultValue)
    {
        return _impl->GetBool(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int Settings::GetInt(const std::string& name, int defaultValue)
    {
        return _impl->GetInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    unsigned int Settings::GetUInt(const std::string& name, unsigned int defaultValue)
    {
        return _impl->GetUInt(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    int64_t Settings::GetInt64(const std::string& name, int64_t defaultValue)
    {
        return _impl->GetInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    uint64_t Settings::GetUInt64(const std::string& name, uint64_t defaultValue)
    {
        return _impl->GetUInt64(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    double Settings::GetDouble(const std::string& name, double defaultValue)
    {
        return _impl->GetDouble(name, defaultValue);
    }
    //--------------------------------------------------------------------------

    std::string Settings::GetString(const std::string& name, const std::string& defaultValue)
    {
        return _impl->GetString(name, defaultValue);
    }
    //--------------------------------------------------------------------------     
}