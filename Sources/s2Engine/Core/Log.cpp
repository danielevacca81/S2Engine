// Log.cpp
//
#include "Log.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace s2 {

static std::shared_ptr<spdlog::logger> s2Logger;

// ------------------------------------------------------------------------------------------------
void Log::init( const std::string& loggerName, LogLevel level )
{
    // es. "[12:34:56] s2Engine: initialization completed"
    spdlog::set_pattern("%^[%T] %n: %v%$");
    
    s2Logger = spdlog::stdout_color_mt(loggerName);
    switch (level) 
    {
    case LogLevel::Trace: s2Logger->set_level(spdlog::level::trace); break;
    case LogLevel::Info:  s2Logger->set_level(spdlog::level::info); break;
    case LogLevel::Warn:  s2Logger->set_level(spdlog::level::warn); break;
    case LogLevel::Error: s2Logger->set_level(spdlog::level::err); break;
    case LogLevel::Fatal: s2Logger->set_level(spdlog::level::critical); break;
    default: s2Logger->set_level(spdlog::level::trace); break;
    }
}


// ------------------------------------------------------------------------------------------------
void Log::logMessage( const LogLevel &level, std::string_view message )
{
    switch (level) 
    {
    case LogLevel::Trace: s2Logger->trace(message); break;
    case LogLevel::Info:  s2Logger->info(message); break;
    case LogLevel::Warn:  s2Logger->warn(message); break;
    case LogLevel::Error: s2Logger->error(message); break;
    case LogLevel::Fatal: s2Logger->critical(message); break;
    }
}

}


