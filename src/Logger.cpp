#include "Logger.h"

const char* const Logger::LevelNames[] = { "TRACE", "DEBUG", "INFO", "CRITICAL" };

// Static public functions
Logger & Logger::GetInstance(void)
{
#ifndef NDEBUG
    static Logger instance(Level::DEBUG);
#else
    static Logger instance;
#endif
    return instance;
}

void          Logger::SetLogLevel(Logger::Level level) { GetInstance().m_logLevel = level; }
Logger::Level Logger::GetLogLevel(void)                { return GetInstance().m_logLevel; }

const std::string Logger::GetLogLevelAsString(void) {
    return std::string(Logger::LevelNames[GetLogLevel()]);
}

const std::string Logger::GetNameOfLogLevel(Level logLevel)
{
    return std::string(Logger::LevelNames[logLevel]);
}

const char* Logger::GetFilepath(void)         { return GetInstance().m_filepath; }
bool        Logger::IsFileOutputEnabled(void) { return GetFilepath() != nullptr; }


bool Logger::EnableFileOutput(const std::string filepath) {
    Logger & loggerInstance = GetInstance();
    loggerInstance.m_filepath = filepath.c_str();
    return loggerInstance.enable_file_output();
}

void Logger::DisableFileOutput(void) { GetInstance().free_file(); }

// Private methods
Logger::Logger(Level logLevel) // Defaults to logLevel = Level::INFO
        :m_logLevel(logLevel)
{
#ifndef NDEBUG
    log(m_logLevel, "Logger initialized. LogLevel set to: %s", LevelNames[m_logLevel]);
#endif
}

Logger::~Logger(void) { free_file(); }

bool Logger::enable_file_output(void)
{
    free_file();

    m_file = std::fopen(m_filepath, "a");
    if (m_file == nullptr) {
        Info("Failed to open file at %s:", m_filepath);
        return false;
    }

    Debug("Started logging into the file: '%s'.", m_filepath);

    return true;
}

void Logger::free_file(void)
{
    if (m_file != nullptr) {
        Debug("Stopped logging into file.");
        std::fclose(m_file);
        m_file = nullptr;
    }
}
