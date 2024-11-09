#ifndef REMOTE_LOGGER_HANDLER_H
#define REMOTE_LOGGER_HANDLER_H

#include "LoggerHandler.h"
#include <string>
#include <stdexcept>

/**
 * @class RemoteLoggerHandler
 * @brief A logging handler that sends log messages to a remote server.
 *
 * This handler connects to a specified server address and port, then sends each log
 * message over the network. Useful for aggregating logs in a centralized logging server.
 */
class RemoteLoggerHandler : public LoggerHandler {
private:
    std::string serverAddress; /**< IP address or hostname of the server */
    int serverPort; /**< Port number for the server */

public:
    /**
     * @brief Constructs a RemoteLoggerHandler with a server address and port.
     *
     * @param address The IP address or hostname of the remote server.
     * @param port The port on which to connect to the server.
     */
    RemoteLoggerHandler(const std::string& address, int port)
        : serverAddress(address), serverPort(port) {}

    /**
     * @brief Logs a message by sending it to the remote server.
     *
     * @param level The severity level of the log (e.g., INFO, DEBUG).
     * @param message The message to log.
     */
    void log(const std::string& level, const std::string& message) override;

private:
    /**
     * @brief Simulated send function to represent network transmission.
     *
     * In a real-world application, this would connect to the server and send
     * the message using a networking library.
     *
     * @param fullMessage The full message to send to the server.
     * @throws std::runtime_error if the message cannot be sent.
     */
    void sendToServer(const std::string& fullMessage);
};

#include "RemoteLoggerHandler.h"
#include <iostream> // Only for simulating network send in this example
inline void RemoteLoggerHandler::log(const std::string& level, const std::string& message) {
    std::string fullMessage = level + ": " + message;
    try {
        sendToServer(fullMessage);
    } catch (const std::runtime_error& e) {
        std::cerr << "Remote logging failed: " << e.what() << std::endl;
    }
}

inline void RemoteLoggerHandler::sendToServer(const std::string& fullMessage) {
    // Simulated network transmission
    std::cout << "Sending to " << serverAddress << ":" << serverPort << " - " << fullMessage << std::endl;

    // In a real implementation, this would establish a network connection
    // and send the fullMessage to the remote server using TCP, UDP, or HTTP.
    // For example:
    // - Using boost::asio for TCP/UDP
    // - Using libcurl for HTTP POST to a logging API
    // This example just simulates a success or failure
    bool simulatedFailure = false; // Change to true to simulate a failure
    if (simulatedFailure) {
        throw std::runtime_error("Network connection failed.");
    }
}
#endif // REMOTE_LOGGER_HANDLER_H
