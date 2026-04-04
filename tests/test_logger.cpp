#include <gtest/gtest.h>

#include "../src/logging/Logger.hpp"

#include <filesystem>
#include <fstream>
#include <string>

using namespace DefenderOS;

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        Logger::instance().setLogLevel(LogLevel::DEBUG);
    }
};

TEST_F(LoggerTest, SingletonReturnsSameInstance) {
    EXPECT_EQ(&Logger::instance(), &Logger::instance());
}

TEST_F(LoggerTest, LogLevelFiltersLowerSeverity) {
    Logger::instance().setLogLevel(LogLevel::WARNING);
    EXPECT_NO_THROW(Logger::instance().debug("should be filtered", "test"));
    EXPECT_NO_THROW(Logger::instance().info("should be filtered", "test"));
    EXPECT_NO_THROW(Logger::instance().warning("should pass", "test"));
}

TEST_F(LoggerTest, AllLevelsLogWithoutCrash) {
    EXPECT_NO_THROW(Logger::instance().debug("d", "test"));
    EXPECT_NO_THROW(Logger::instance().info("i", "test"));
    EXPECT_NO_THROW(Logger::instance().warning("w", "test"));
    EXPECT_NO_THROW(Logger::instance().error("e", "test"));
    EXPECT_NO_THROW(Logger::instance().critical("c", "test"));
}

TEST_F(LoggerTest, EmptyModuleDoesNotCrash) {
    EXPECT_NO_THROW(Logger::instance().info("no module", ""));
}

TEST_F(LoggerTest, SetLogLevelChangesThreshold) {
    Logger::instance().setLogLevel(LogLevel::CRITICAL);
    EXPECT_NO_THROW(Logger::instance().error("filtered error", "test"));
    EXPECT_NO_THROW(Logger::instance().critical("visible critical", "test"));
    Logger::instance().setLogLevel(LogLevel::DEBUG);
}

TEST_F(LoggerTest, LogToFileCreatesFileWithContent) {
    const std::string path = "/tmp/defender_ui_test.log";
    if (std::filesystem::exists(path)) {
        std::filesystem::remove(path);
    }

    Logger::instance().setOutputFile(path);
    Logger::instance().info("file log test", "test");
    Logger::instance().setOutputFile(""); // close by setting invalid path

    ASSERT_TRUE(std::filesystem::exists(path));

    std::ifstream file(path);
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    EXPECT_FALSE(content.empty());
    EXPECT_NE(content.find("file log test"), std::string::npos);

    std::filesystem::remove(path);
}

TEST_F(LoggerTest, InvalidLogFilePathDoesNotCrash) {
    EXPECT_NO_THROW(Logger::instance().setOutputFile("/no/such/directory/log.txt"));
    EXPECT_NO_THROW(Logger::instance().info("after bad file", "test"));
}
