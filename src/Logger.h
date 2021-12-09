#ifndef LOGGER_H
#define LOGGER_H

#include "global.h"

#include <memory>
#include <string>
#include <stdexcept>
#include <cstdio>
#include <ctime>
#include <mutex>

// TODO: Use IO-class for file & stdout operations.
class Logger
{
public:
    static char const * const LevelNames[];
    enum Level {
        // When changing the possible levels in this enum also edit the array LevelNames[]
        // containing the names for every level in addition to all the static functions.
        TRACE = 0, DEBUG, INFO, CRITICAL
    };

    Logger(const Logger& other) = delete;
    Logger& operator=(const Logger& other) = delete;

    static Logger&           GetInstance(void);
    static void              SetLogLevel(Level level);
    static Level             GetLogLevel(void);
    static const std::string GetLogLevelAsString(void);
    static const std::string GetNameOfLogLevel(Level logLevel);
    static const char*       GetFilepath(void);
    static bool              IsFileOutputEnabled(void);
    static bool              EnableFileOutput(const std::string filepath);
    static void              DisableFileOutput(void);

    template<typename... Args>
    static void Trace(const std::string & message, Args... args)
    {
        GetInstance().log(Level::TRACE, message.c_str(), args...);
    }

    template<typename... Args>
    static void Debug(const std::string & message, Args... args)
    {
        GetInstance().log(Level::DEBUG, message.c_str(), args...);
    }

    template<typename... Args>
    static void Info(const std::string & message, Args... args)
    {
        GetInstance().log(Level::INFO, message.c_str(), args...);
    }

    template<typename... Args>
    static void Critical(const std::string & message, Args... args)
    {
        GetInstance().log(Level::CRITICAL, message.c_str(), args...);
    }

private:
    Level               m_logLevel;
    mutable std::mutex  m_log_mutex;
    const char *        m_filepath;
    FILE *              m_file;

    Logger(Level logLevel = Level::INFO);
    ~Logger(void);

    bool enable_file_output(void);
    void free_file(void);

    template<typename... Args>
    void log(Level messagePriority, const char * message, Args... args) const
    {
        // TODO: Use boost::format or C++20 format for a more robust way to
        // construct the final string containing the varargs
        if (m_logLevel > messagePriority) {
            return;
        }

        const std::time_t current_time = std::time(nullptr);
        const std::tm * timestamp = std::localtime(&current_time);
        char timestampFormatted[9];
/* ---------------------------------------------------------------
 * Use this code to format the final string if using cout streams
        const char * messChar = message;
        std::unique_ptr<char[]> messageFormattedPtr;

        // Check if message is a format string and construct the string into
        // the c-string pointed to by messageFormattedPtr
        while (*messChar != '\0') {
            if (*messChar == '%') {
                // Handle & format variadic args
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
#endif
                int size_s = std::snprintf(nullptr, 0, message, args...) + 1;
                if (size_s <= 0) {
                    throw std::runtime_error("Error formatting string for logging");
                }

                auto size = static_cast<size_t>(size_s);
                messageFormattedPtr = std::make_unique<char[]>(size);
                std::snprintf(messageFormattedPtr.get(), size, message, args...);
#ifdef __clang__
#pragma clang diagnostic pop
#endif
                break;
            }
            ++messChar;
        }
--------------------------------------------------------------- */

        if (strftime(timestampFormatted, 9, "%T", timestamp) == 0) {
            throw std::runtime_error("Error constructing timestamp for log data");
        }

        std::lock_guard<std::mutex> lock(m_log_mutex);
        printf("[%s] [%s]:\t",
               timestampFormatted,
               Logger::LevelNames[messagePriority]
        );
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
#endif
        printf(message, args...);
        printf("\n");

        if (m_file != nullptr) {
            fprintf(m_file, "[%s] [%s]:\t",
                    timestampFormatted,
                    Logger::LevelNames[messagePriority]
            );
            fprintf(m_file, message, args...);
            fprintf(m_file, "\n");
        }
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    }
};

#endif
