/**
 * @file Logger.cpp
 * @brief Implementation of Logger class
 */

#include "Logger.h"
#include <cstdarg>
#include <ctime>
#include <iostream>
#include <filesystem>

// Undefine Windows macros that conflict with our enums
#ifdef ERROR
#undef ERROR
#endif
#ifdef SUCCESS
#undef SUCCESS
#endif

// Static member initialization
std::mutex Logger::s_mutex;
std::ofstream Logger::s_logFile;
LogLevel Logger::s_minLogLevel = LogLevel::DEBUG;
bool Logger::s_fileLoggingEnabled = true;
bool Logger::s_initialized = false;

bool Logger::Initialize()
{
    std::lock_guard<std::mutex> lock(s_mutex);

    if (s_initialized)
        return true;

    // Create logs directory if it doesn't exist
    std::filesystem::create_directories("../logs");

    // Open log file with timestamp
    std::string filename = "../logs/jx1auto_" + GetTimestamp() + ".log";
    s_logFile.open(filename, std::ios::out | std::ios::app);

    if (!s_logFile.is_open())
    {
        std::cerr << "[ERROR] Failed to open log file: " << filename << std::endl;
        return false;
    }

    s_initialized = true;

    // Write header
    s_logFile << "========================================" << std::endl;
    s_logFile << "JX1 Auto Core - Log File" << std::endl;
    s_logFile << "Started: " << GetTimestamp() << std::endl;
    s_logFile << "========================================" << std::endl;
    s_logFile.flush();

    return true;
}

void Logger::Shutdown()
{
    std::lock_guard<std::mutex> lock(s_mutex);

    if (s_logFile.is_open())
    {
        s_logFile << "========================================" << std::endl;
        s_logFile << "Log ended: " << GetTimestamp() << std::endl;
        s_logFile << "========================================" << std::endl;
        s_logFile.close();
    }

    s_initialized = false;
}

void Logger::Log(LogLevel level, const char* format, ...)
{
    // Check if we should log this level
    if (level < s_minLogLevel)
        return;

    // Format the message
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    std::lock_guard<std::mutex> lock(s_mutex);

    // Get timestamp and level string
    std::string timestamp = GetTimestamp();
    const char* levelStr = GetLogLevelString(level);

    // Console output with color
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD originalColor;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if (GetConsoleScreenBufferInfo(hConsole, &csbi))
    {
        originalColor = csbi.wAttributes;
    }
    else
    {
        originalColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }

    SetConsoleTextAttribute(hConsole, GetConsoleColor(level));
    printf("[%s] [%s] %s\n", timestamp.c_str(), levelStr, buffer);
    SetConsoleTextAttribute(hConsole, originalColor);

    // File output
    if (s_fileLoggingEnabled && s_logFile.is_open())
    {
        s_logFile << "[" << timestamp << "] [" << levelStr << "] " << buffer << std::endl;
        s_logFile.flush();
    }
}

void Logger::Debug(const char* format, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Log(LogLevel::DEBUG, "%s", buffer);
}

void Logger::Info(const char* format, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Log(LogLevel::INFO, "%s", buffer);
}

void Logger::Warning(const char* format, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Log(LogLevel::WARNING, "%s", buffer);
}

void Logger::Error(const char* format, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Log(LogLevel::ERROR, "%s", buffer);
}

void Logger::Success(const char* format, ...)
{
    char buffer[4096];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    Log(LogLevel::SUCCESS, "%s", buffer);
}

void Logger::SetLogLevel(LogLevel level)
{
    s_minLogLevel = level;
}

void Logger::SetFileLogging(bool enabled)
{
    s_fileLoggingEnabled = enabled;
}

WORD Logger::GetConsoleColor(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:
        return FOREGROUND_INTENSITY;  // Gray
    case LogLevel::INFO:
        return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;  // White
    case LogLevel::WARNING:
        return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;  // Yellow
    case LogLevel::ERROR:
        return FOREGROUND_RED | FOREGROUND_INTENSITY;  // Red
    case LogLevel::SUCCESS:
        return FOREGROUND_GREEN | FOREGROUND_INTENSITY;  // Green
    default:
        return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
}

const char* Logger::GetLogLevelString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::DEBUG:   return "DEBUG";
    case LogLevel::INFO:    return "INFO ";
    case LogLevel::WARNING: return "WARN ";
    case LogLevel::ERROR:   return "ERROR";
    case LogLevel::SUCCESS: return "OK   ";
    default:                return "UNKNOWN";
    }
}

std::string Logger::GetTimestamp()
{
    time_t now = time(nullptr);
    struct tm timeinfo;
    localtime_s(&timeinfo, &now);

    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    return std::string(buffer);
}
