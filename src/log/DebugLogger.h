#ifndef DEBUG_LOGGER_H
#define DEBUG_LOGGER_H

#include <string>
#include <vector>
#include <mutex>
#include <sstream>
#include <memory>
#include "LoggerHandler.h"

/**
 * @class DebugLogger
 * @brief A singleton logging system that manages multiple handlers.
 *
 * Allows attaching different handlers (e.g., console, file) to output log messages
 * to various destinations. Provides thread-safe logging and formatted message support.
 */
class DebugLogger {
private:
    std::vector<std::unique_ptr<LoggerHandler>> handlers; /**< List of logging handlers */
    std::mutex logMutex; /**< Mutex for synchronizing log output */

    /**
     * @brief Private constructor for singleton pattern.
     */
    DebugLogger() = default;

public:
    /**
     * @brief Gets the singleton instance of DebugLogger.
     *
     * @return DebugLogger& Reference to the singleton instance.
     */
    static DebugLogger& getInstance();

    /**
     * @brief Adds a new logging handler.
     *
     * @param handler A unique_ptr to a LoggerHandler instance.
     */
    void addHandler(std::unique_ptr<LoggerHandler> handler);

    /**
     * @brief Logs a message with a specific level to all attached handlers.
     *
     * @tparam Args Variadic template for multiple arguments.
     * @param level The severity level of the log (e.g., INFO, DEBUG).
     * @param args Variadic arguments to construct the log message.
     */
    template<typename... Args>
    void log(const std::string& level, Args... args);
};

// Macro for simplified logging with variadic arguments
#define LOGL(level, ...) DebugLogger::getInstance().log(level, __VA_ARGS__)

// Implementation of DebugLogger methods

DebugLogger& DebugLogger::getInstance() {
    static DebugLogger instance;
    return instance;
}

void DebugLogger::addHandler(std::unique_ptr<LoggerHandler> handler) {
    std::lock_guard<std::mutex> lock(logMutex);
    handlers.push_back(std::move(handler));
}

template<typename... Args>
void DebugLogger::log(const std::string& level, Args... args) {
    std::ostringstream stream;
    (stream << ... << args);
    std::string message = stream.str();

    std::lock_guard<std::mutex> lock(logMutex);
    for (auto& handler : handlers) {
        handler->log(level, message);
    }
}

#endif // DEBUG_LOGGER_H
