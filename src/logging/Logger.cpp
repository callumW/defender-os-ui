#include "Logger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace DefenderOS {

Logger& Logger::instance() {
    static Logger singleton;
    return singleton;
}

Logger::~Logger() {
    if (m_fileStream && m_fileStream->is_open()) {
        m_fileStream->close();
    }
}

void Logger::setLogLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_minLevel = level;
}

void Logger::setOutputFile(const std::string& filePath) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_fileStream = std::make_unique<std::ofstream>(filePath, std::ios::app);
    if (!m_fileStream->is_open()) {
        std::cerr << "[Logger] Failed to open log file: " << filePath << "\n";
        m_fileStream.reset();
    }
}

void Logger::debug(const std::string& message, const std::string& module) {
    log(LogLevel::DEBUG, message, module);
}

void Logger::info(const std::string& message, const std::string& module) {
    log(LogLevel::INFO, message, module);
}

void Logger::warning(const std::string& message, const std::string& module) {
    log(LogLevel::WARNING, message, module);
}

void Logger::error(const std::string& message, const std::string& module) {
    log(LogLevel::ERROR, message, module);
}

void Logger::critical(const std::string& message, const std::string& module) {
    log(LogLevel::CRITICAL, message, module);
}

void Logger::log(LogLevel level, const std::string& message, const std::string& module) {
    if (level < m_minLevel) {
        return;
    }

    std::lock_guard<std::mutex> lock(m_mutex);
    const std::string formatted = formatMessage(level, message, module);

    std::cout << formatted << "\n";

    if (m_fileStream && m_fileStream->is_open()) {
        *m_fileStream << formatted << "\n";
        m_fileStream->flush();
    }
}

std::string Logger::formatMessage(LogLevel level,
                                  const std::string& message,
                                  const std::string& module) const {
    std::ostringstream oss;
    oss << "[" << currentTimestamp() << "] "
        << "[" << levelToString(level) << "]";
    if (!module.empty()) {
        oss << " [" << module << "]";
    }
    oss << " " << message;
    return oss.str();
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:    return "DEBUG";
        case LogLevel::INFO:     return "INFO";
        case LogLevel::WARNING:  return "WARNING";
        case LogLevel::ERROR:    return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
    }
    return "UNKNOWN";
}

std::string Logger::currentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t nowT = std::chrono::system_clock::to_time_t(now);
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                        now.time_since_epoch()) % 1000;

    std::tm tmBuf{};
#ifdef _WIN32
    localtime_s(&tmBuf, &nowT);
#else
    localtime_r(&nowT, &tmBuf);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tmBuf, "%Y-%m-%d %H:%M:%S")
        << "." << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}

} // namespace DefenderOS
