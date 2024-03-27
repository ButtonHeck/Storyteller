#include "log.h"
#include "string_utils.h"

#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Storyteller
{
    Ptr<spdlog::logger> Log::_coreLogger;
    Ptr<spdlog::logger> Log::_clientLogger;
    std::stringstream Log::_ss;

    void Log::Initialize(const LogConfig& config)
    {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(CreatePtr<spdlog::sinks::ostream_sink_mt>(_ss));
        logSinks.emplace_back(CreatePtr<spdlog::sinks::basic_file_sink_mt>(config.name, config.truncate));

        logSinks[0]->set_pattern("[%T.%e] [%l] %n: %v");
        logSinks[1]->set_pattern("[%T.%e] [%l] %n: %v");

        _coreLogger = CreatePtr<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
        _coreLogger->set_level(spdlog::level::trace);
        _coreLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(_coreLogger);

        _clientLogger = CreatePtr<spdlog::logger>("CLIENT", begin(logSinks), end(logSinks));
        _clientLogger->set_level(spdlog::level::trace);
        _clientLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(_clientLogger);
    }
    //--------------------------------------------------------------------------

    Ptr<spdlog::logger>& Log::CoreLogger()
    {
        return _coreLogger;
    }
    //--------------------------------------------------------------------------

    Ptr<spdlog::logger>& Log::ClientLogger()
    {
        return _clientLogger;
    }
    //--------------------------------------------------------------------------

    std::string_view Log::StringLogOutput()
    {
        return _ss.view();
    }
    //--------------------------------------------------------------------------
}