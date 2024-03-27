#pragma once

#include "log.h"
#include "filesystem.h"

#include <rapidjson/document.h>

#include <string>

namespace Storyteller
{
    class JsonReader;

    class Config
    {
    public:
        Config();

        bool Load(const std::filesystem::path& path);

        const LogConfig& GetLogConfig() const;

    private:
        void LoadLogConfig();

    private:
        rapidjson::Document _document;
        LogConfig _log;
    };
}