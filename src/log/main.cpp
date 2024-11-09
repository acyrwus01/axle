#include "DebugLogger.h"
#include "ConsoleLoggerHandler.h"
#include "FileLoggerHandler.h"
#include <thread>
#include <iostream>

void logSampleMessages(const std::string& prefix) {
    LOGL("INFO", prefix, " System initialized with value: ", 42, ", status: ", true);
    LOGL("DEBUG", prefix, " User coordinates: ", 10.0, ", ", 20.0);
    LOGL("WARNING", prefix, " Memory usage is above threshold: ", 75, "%");
    LOGL("ERROR", prefix, " Critical failure in module ", "network");
}

int main() {
    auto& logger = DebugLogger::getInstance();

    // Test 1: Console and File Logging
    logger.addHandler(std::make_unique<ConsoleLoggerHandler>());
    try {
        logger.addHandler(std::make_unique<FileLoggerHandler>("logfile.log"));
    } catch (const std::runtime_error& e) {
        std::cerr << "File Logger Initialization Error: " << e.what() << std::endl;
    }

    // Test 2: Basic logging output
    logSampleMessages("BasicTest");

    // Test 3: Concurrent logging
    std::thread t1(logSampleMessages, "Thread1");
    std::thread t2(logSampleMessages, "Thread2");
    t1.join();
    t2.join();

    // Test 4: Error Handling (attempting to write to an invalid directory)
    try {
        logger.addHandler(std::make_unique<FileLoggerHandler>("/invalid_directory/logfile.log"));
    } catch (const std::runtime_error& e) {
        std::cerr << "File Logger Error: " << e.what() << std::endl;
    }

    return 0;
}
