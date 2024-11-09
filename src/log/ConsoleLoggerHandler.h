#ifndef CONSOLE_LOGGER_HANDLER_H
#define CONSOLE_LOGGER_HANDLER_H

#include <iostream>
#include "LoggerHandler.h"

#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define ORANGE  "\033[38;5;214m" /* Orange */
#define RED     "\033[31m"      /* Red */
#define RESET   "\033[0m"

/**
 * @class ConsoleLoggerHandler
 * @brief A logging handler that outputs log messages to the console.
 *
 * Supports color-coded log messages based on the log level.
 */
class ConsoleLoggerHandler : public LoggerHandler {
public:
    /**
     * @brief Logs a message to the console with color coding.
     *
     * @param level The severity level of the log (e.g., INFO, DEBUG).
     * @param message The message to log.
     */
    void log(const std::string& level, const std::string& message) override;
};

// Implementation of ConsoleLoggerHandler::log
void ConsoleLoggerHandler::log(const std::string& level, const std::string& message) {
    std::string color = RESET;
    if (level == "INFO") color = GREEN;
    else if (level == "DEBUG") color = YELLOW;
    else if (level == "WARNING") color = ORANGE;
    else if (level == "ERROR") color = RED;

    std::cout << color << level << ": " << message << RESET << std::endl;
}

#endif // CONSOLE_LOGGER_HANDLER_H
