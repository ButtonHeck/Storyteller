#pragma once

#include "pointers.h"
#include "settings_json_impl_reader.h"
#include "settings_json_impl_writer.h"

#include <string>

namespace Storyteller
{
    class SettingsJson
    {
    public:
        explicit SettingsJson(const std::string& name);

        bool StartSave();
        bool EndSave();

        bool StartSaveGroup(const std::string& groupName);
        bool EndSaveGroup();

        bool StartSaveArray(const std::string& arrayName);
        bool EndSaveArray();

        bool SaveBool(bool value);
        bool SaveBool(const std::string& name, bool value);
        bool SaveInt(int value);
        bool SaveInt(const std::string& name, int value);
        bool SaveUInt(unsigned int value);
        bool SaveUInt(const std::string& name, unsigned int value);
        bool SaveInt64(int64_t value);
        bool SaveInt64(const std::string& name, int64_t value);
        bool SaveUInt64(uint64_t value);
        bool SaveUInt64(const std::string& name, uint64_t value);
        bool SaveDouble(double value);
        bool SaveDouble(const std::string& name, double value);
        bool SaveString(const std::string& value);
        bool SaveString(const std::string& name, const std::string& value);


        bool StartLoad();
        bool EndLoad();

        bool StartLoadGroup(const std::string& groupName);
        bool EndLoadGroup();

        int StartLoadArray(const std::string& arrayName);
        bool EndLoadArray();

        bool GetBool(int index, bool defaultValue = false);
        bool GetBool(const std::string& name, bool defaultValue = false);
        int GetInt(int index, int defaultValue = 0);
        int GetInt(const std::string& name, int defaultValue = 0);
        unsigned int GetUInt(int index, unsigned int defaultValue = 0);
        unsigned int GetUInt(const std::string& name, unsigned int defaultValue = 0);
        int64_t GetInt64(int index, int64_t defaultValue = 0);
        int64_t GetInt64(const std::string& name, int64_t defaultValue = 0);
        uint64_t GetUInt64(int index, uint64_t defaultValue = 0);
        uint64_t GetUInt64(const std::string& name, uint64_t defaultValue = 0);
        double GetDouble(int index, double defaultValue = 0.0);
        double GetDouble(const std::string& name, double defaultValue = 0.0);
        std::string GetString(int index, const std::string& defaultValue = "");
        std::string GetString(const std::string& name, const std::string& defaultValue = "");

    private:
        Ptr<SettingsJsonReader> _reader;
        Ptr<SettingsJsonWriter> _writer;
    };
    //--------------------------------------------------------------------------
}