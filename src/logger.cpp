#include "defender/logger.h"
#include <iostream>
#include <ctime>
#include <iomanip>

namespace defender {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : minLogLevel_(LogLevel::INFO) {}

void Logger::setLogLevel(LogLevel level) {
    minLogLevel_ = level;
}

LogLevel Logger::getLogLevel() const {
    return minLogLevel_;
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::log(LogLevel level, const std::string& message) {
    // Only log if the level is at or above the minimum level
    if (level < minLogLevel_) {
        return;
    }

    // Get current time
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);

    // Format and output log message
    std::cerr << "[" << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] "
              << "[" << levelToString(level) << "] "
              << message << std::endl;
}

const char* Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

} // namespace defender
