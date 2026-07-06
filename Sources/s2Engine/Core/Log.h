// Log.h
//
#ifndef S2LOG_ONCE
#define S2LOG_ONCE

#include "s2Engine_API.h"

#include <memory>
#include <format>
#include <string_view>
#include <string>

namespace s2 {

enum class LogLevel
{
    Trace,
    Info,
    Warn,
    Error,
    Fatal,
};

struct LogParams
{
    struct FileLogger
    {
        uint64_t maxSize       { 0 };
        uint64_t maxFilesCount { 0 };
        bool     rotating      { false };
        bool     enabled       { false };
    };

    struct ConsoleLogger
    {
        bool enabled { true };
    };


    ConsoleLogger  console;
    FileLogger     file;

    std::string    pattern;
};

class S2ENGINE_API Log
{

public:
    static void init( const std::string& loggerName = "s2Engine", const LogLevel &level = LogLevel::Trace, const LogParams &params = {});
    static void logMessage( const LogLevel &level, std::string_view message );
};

}

#define LOG( level, ...) \
    ::s2::Log::logMessage(s2::LogLevel::level, std::format(__VA_ARGS__))

#define LOG_IF(condition, level, ...) \
    do { \
        if (condition) { \
            ::s2::Log::logMessage(s2::LogLevel::level, std::format(__VA_ARGS__)); \
        } \
    } while(false)


#if defined(_WIN32)
    #define FMT_UNICODE 0
    #define S2_DEBUG_BREAK() __debugbreak()
#elif defined(__linux__)
    #include <signal.h>
    #define S2_DEBUG_BREAK() raise(SIGTRAP)
#else
    #define S2_DEBUG_BREAK()
#endif

#ifdef _DEBUG
    #define S2_ASSERT(condition, ...) \
        do { \
            if (!(condition)) { \
                LOG( Fatal, "ASSERTION FAILED: {}", std::format(__VA_ARGS__)); \
                S2_DEBUG_BREAK(); \
            } \
        } while(false)
#else
    #define S2_ASSERT(condition, ...) 
#endif    

    
#endif // !S2LOG_ONCE