#pragma once

#include "platform.h"
#include "pointers.h"

#if defined (STRTLR_COMPILER_MSVC)
#pragma warning(disable : 4996)
#endif
#include <spdlog/spdlog.h>

#include <sstream>

namespace Storyteller
{
    struct LogConfig
    {
        std::string filename = "Storyteller.log";
        bool enabled = true;
        bool truncate = false;
        bool outputConsole = false;
        bool outputFile = true;
        bool outputStringBuffer = true;
        int coreLevel = spdlog::level::trace;
        int clientLevel = spdlog::level::trace;
    };
    //--------------------------------------------------------------------------

    class Log
    {
    public:
        static void Initialize(const LogConfig& config);

        static Ptr<spdlog::logger>& CoreLogger();
        static Ptr<spdlog::logger>& ClientLogger();
        static std::string_view StringLogOutput();

    private:
        static Ptr<spdlog::logger> _coreLogger;
        static Ptr<spdlog::logger> _clientLogger;
        static std::stringstream _ss;
    };
    //--------------------------------------------------------------------------
}

#define STRTLR_CORE_LOG_TRACE(...)       ::Storyteller::Log::CoreLogger()->trace(__VA_ARGS__)
#define STRTLR_CORE_LOG_DEBUG(...)       ::Storyteller::Log::CoreLogger()->debug(__VA_ARGS__)
#define STRTLR_CORE_LOG_INFO(...)        ::Storyteller::Log::CoreLogger()->info(__VA_ARGS__)
#define STRTLR_CORE_LOG_WARN(...)        ::Storyteller::Log::CoreLogger()->warn(__VA_ARGS__)
#define STRTLR_CORE_LOG_ERROR(...)       ::Storyteller::Log::CoreLogger()->error(__VA_ARGS__)
#define STRTLR_CORE_LOG_CRITICAL(...)    ::Storyteller::Log::CoreLogger()->critical(__VA_ARGS__)

#define STRTLR_CLIENT_LOG_TRACE(...)     ::Storyteller::Log::ClientLogger()->trace(__VA_ARGS__)
#define STRTLR_CLIENT_LOG_DEBUG(...)     ::Storyteller::Log::ClientLogger()->debug(__VA_ARGS__)
#define STRTLR_CLIENT_LOG_INFO(...)      ::Storyteller::Log::ClientLogger()->info(__VA_ARGS__)
#define STRTLR_CLIENT_LOG_WARN(...)      ::Storyteller::Log::ClientLogger()->warn(__VA_ARGS__)
#define STRTLR_CLIENT_LOG_ERROR(...)     ::Storyteller::Log::ClientLogger()->error(__VA_ARGS__)
#define STRTLR_CLIENT_LOG_CRITICAL(...)  ::Storyteller::Log::ClientLogger()->critical(__VA_ARGS__)