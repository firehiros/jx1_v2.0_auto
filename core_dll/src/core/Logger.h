/**
 * @file Logger.h
 * @brief Logging system for JX1 Auto Core
 *
 * Provides console and file logging with multiple log levels.
 */

#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <Windows.h>

/**
 * Log levels
 */
enum class LogLevel
{
    DEBUG,      // Detailed debug information
    INFO,       // General information
    WARNING,    // Warning messages
    ERROR,      // Error messages
    SUCCESS     // Success messages (with green color)
};

/**
 * Logger class
 *
 * Thread-safe logging to console and file.
 */
class Logger
{
public:
    /**
     * Initialize logger
     * @return true if successful
     */
    static bool Initialize();

    /**
     * Shutdown logger
     */
    static void Shutdown();

    /**
     * Log a message with specific level
     * @param level Log level
     * @param format Format string (printf style)
     * @param ... Variable arguments
     */
    static void Log(LogLevel level, const char* format, ...);

    /**
     * Convenience functions for each log level
     */
    static void Debug(const char* format, ...);
    static void Info(const char* format, ...);
    static void Warning(const char* format, ...);
    static void Error(const char* format, ...);
    static void Success(const char* format, ...);

    /**
     * Set minimum log level
     * Messages below this level won't be logged
     */
    static void SetLogLevel(LogLevel level);

    /**
     * Enable/disable file logging
     */
    static void SetFileLogging(bool enabled);

private:
    static std::mutex s_mutex;
    static std::ofstream s_logFile;
    static LogLevel s_minLogLevel;
    static bool s_fileLoggingEnabled;
    static bool s_initialized;

    /**
     * Get color for console output
     */
    static WORD GetConsoleColor(LogLevel level);

    /**
     * Get string representation of log level
     */
    static const char* GetLogLevelString(LogLevel level);

    /**
     * Get current timestamp
     */
    static std::string GetTimestamp();
};
