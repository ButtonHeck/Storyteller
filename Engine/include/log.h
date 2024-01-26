#pragma once

#ifdef _WIN32
#define NOGDI
#include <Windows.h>
#pragma warning(disable : 4996)
#endif
#include <spdlog/spdlog.h>

#include <memory>
#include <sstream>

namespace Storyteller
{
    class Log
    {
    public:
        static void Initialize();

        static std::shared_ptr<spdlog::logger>& CoreLogger();
        static std::shared_ptr<spdlog::logger>& ClientLogger();
        static std::string_view StringLogOutput();

    private:
        static std::shared_ptr<spdlog::logger> _coreLogger;
        static std::shared_ptr<spdlog::logger> _clientLogger;
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