#ifndef FILE_LOGGER_HANDLER_H
#define FILE_LOGGER_HANDLER_H

#include <fstream>
#include "LoggerHandler.h"

/**
 * @class FileLoggerHandler
 * @brief A logging handler that writes log messages to a file.
 *
 * Opens the specified file in append mode to ensure that logs are added without overwriting
 * previous entries.
 */
class FileLoggerHandler : public LoggerHandler {
private:
    std::ofstream logFile; /**< File stream for the log file */

public:
    /**
     * @brief Constructs a FileLoggerHandler and opens the specified file.
     *
     * @param filename The name of the file to write logs to.
     * @throws std::runtime_error if the file cannot be opened.
     */
    explicit FileLoggerHandler(const std::string& filename);

    /**
     * @brief Destructor closes the file if open.
     */
    ~FileLoggerHandler();

    /**
     * @brief Logs a message to the file.
     *
     * @param level The severity level of the log (e.g., INFO, DEBUG).
     * @param message The message to log.
     */
    void log(const std::string& level, const std::string& message) override;
};

// Implementation of FileLoggerHandler methods

FileLoggerHandler::FileLoggerHandler(const std::string& filename) {
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        throw std::runtime_error("Unable to open log file: " + filename);
    }
}

FileLoggerHandler::~FileLoggerHandler() {
    if (logFile.is_open()) {
        logFile.close();
    }
}

void FileLoggerHandler::log(const std::string& level, const std::string& message) {
    if (logFile.is_open()) {
        logFile << level << ": " << message << std::endl;
    }
}

#endif // FILE_LOGGER_HANDLER_H
