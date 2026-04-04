#pragma once

#include <fstream>
#include <memory>
#include <mutex>
#include <string>

namespace DefenderOS {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

/// Thread-safe singleton logger used by all modules.
///
/// Obtain the instance with Logger::instance() and call the level-specific
/// helpers, or use the LOG_* convenience macros defined below.
class Logger {
public:
    [[nodiscard]] static Logger& instance();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /// Messages below this level are silently discarded.
    void setLogLevel(LogLevel level);

    /// Open @p filePath for appended log output (in addition to stdout).
    void setOutputFile(const std::string& filePath);

    void debug(const std::string& message, const std::string& module = "");
    void info(const std::string& message, const std::string& module = "");
    void warning(const std::string& message, const std::string& module = "");
    void error(const std::string& message, const std::string& module = "");
    void critical(const std::string& message, const std::string& module = "");

private:
    Logger() = default;
    ~Logger();

    void log(LogLevel level, const std::string& message, const std::string& module);

    [[nodiscard]] std::string formatMessage(LogLevel level,
                                            const std::string& message,
                                            const std::string& module) const;
    [[nodiscard]] static std::string levelToString(LogLevel level);
    [[nodiscard]] static std::string currentTimestamp();

    LogLevel m_minLevel{LogLevel::DEBUG};
    std::unique_ptr<std::ofstream> m_fileStream;
    mutable std::mutex m_mutex;
};

// ---------------------------------------------------------------------------
// Convenience macros — include file name automatically as the module tag.
// ---------------------------------------------------------------------------
#define LOG_DEBUG(msg)    DefenderOS::Logger::instance().debug((msg), __FILE__)
#define LOG_INFO(msg)     DefenderOS::Logger::instance().info((msg), __FILE__)
#define LOG_WARNING(msg)  DefenderOS::Logger::instance().warning((msg), __FILE__)
#define LOG_ERROR(msg)    DefenderOS::Logger::instance().error((msg), __FILE__)
#define LOG_CRITICAL(msg) DefenderOS::Logger::instance().critical((msg), __FILE__)

} // namespace DefenderOS
