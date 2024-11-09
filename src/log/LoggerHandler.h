#ifndef LOGGER_HANDLER_H
#define LOGGER_HANDLER_H

#include <string>

/**
 * @class LoggerHandler
 * @brief Abstract base class for different logging handlers.
 *
 * This interface allows different types of logging (e.g., console, file) by implementing
 * the `log` method to output log messages.
 */
class LoggerHandler {
public:
    /**
     * @brief Virtual destructor for base class.
     */
    virtual ~LoggerHandler() = default;

    /**
     * @brief Logs a message with a specific log level.
     *
     * @param level The severity level of the log (e.g., INFO, DEBUG).
     * @param message The message to log.
     */
    virtual void log(const std::string& level, const std::string& message) = 0;
};

#endif // LOGGER_HANDLER_H
