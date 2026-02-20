#pragma once

#include <string>
#include <memory>
#include <sstream>

namespace defender {

/**
 * @brief Log levels for the logging system
 */
enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

/**
 * @brief Logger interface for unified logging across all modules
 * 
 * This class provides a centralized logging system with different log levels.
 * It follows the Singleton pattern to ensure consistent logging throughout
 * the application.
 */
class Logger {
public:
    /**
     * @brief Get the singleton instance of the logger
     */
    static Logger& getInstance();

    /**
     * @brief Set the minimum log level to display
     * @param level The minimum log level
     */
    void setLogLevel(LogLevel level);

    /**
     * @brief Get the current log level
     */
    LogLevel getLogLevel() const;

    /**
     * @brief Log a debug message
     * @param message The message to log
     */
    void debug(const std::string& message);

    /**
     * @brief Log an info message
     * @param message The message to log
     */
    void info(const std::string& message);

    /**
     * @brief Log a warning message
     * @param message The message to log
     */
    void warning(const std::string& message);

    /**
     * @brief Log an error message
     * @param message The message to log
     */
    void error(const std::string& message);

    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger();
    ~Logger() = default;

    void log(LogLevel level, const std::string& message);
    const char* levelToString(LogLevel level) const;

    LogLevel minLogLevel_;
};

} // namespace defender
