#include "defender/logger.h"
#include <cassert>
#include <iostream>

using namespace defender;

void testLoggerSingleton() {
    Logger& logger1 = Logger::getInstance();
    Logger& logger2 = Logger::getInstance();
    
    // Both references should point to the same instance
    assert(&logger1 == &logger2);
    std::cout << "✓ Logger singleton test passed" << std::endl;
}

void testLogLevels() {
    Logger& logger = Logger::getInstance();
    
    // Test setting and getting log level
    logger.setLogLevel(LogLevel::DEBUG);
    assert(logger.getLogLevel() == LogLevel::DEBUG);
    
    logger.setLogLevel(LogLevel::WARNING);
    assert(logger.getLogLevel() == LogLevel::WARNING);
    
    std::cout << "✓ Log level test passed" << std::endl;
}

void testLogging() {
    Logger& logger = Logger::getInstance();
    logger.setLogLevel(LogLevel::DEBUG);
    
    // These should not crash
    logger.debug("Debug message");
    logger.info("Info message");
    logger.warning("Warning message");
    logger.error("Error message");
    
    std::cout << "✓ Logging test passed" << std::endl;
}

int main() {
    std::cout << "Running Logger tests..." << std::endl;
    
    try {
        testLoggerSingleton();
        testLogLevels();
        testLogging();
        
        std::cout << "All Logger tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
