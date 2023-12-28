#include "log.h"

#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Storyteller
{
    std::shared_ptr<spdlog::logger> Log::_coreLogger;
    std::shared_ptr<spdlog::logger> Log::_clientLogger;
    std::stringstream Log::_ss;

    void Log::Initialize()
    {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::ostream_sink_mt>(_ss));
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Storyteller.log", true));

        logSinks[0]->set_pattern("[%T.%e] [%l] %n: %v");
        logSinks[1]->set_pattern("[%T.%e] [%l] %n: %v");

        _coreLogger = std::make_shared<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
        _coreLogger->set_level(spdlog::level::trace);
        _coreLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(_coreLogger);

        _clientLogger = std::make_shared<spdlog::logger>("CLIENT", begin(logSinks), end(logSinks));
        _clientLogger->set_level(spdlog::level::trace);
        _clientLogger->flush_on(spdlog::level::trace);
        spdlog::register_logger(_clientLogger);
    }
    //--------------------------------------------------------------------------

    std::shared_ptr<spdlog::logger>& Log::CoreLogger()
    {
        return _coreLogger;
    }
    //--------------------------------------------------------------------------

    std::shared_ptr<spdlog::logger>& Log::ClientLogger()
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