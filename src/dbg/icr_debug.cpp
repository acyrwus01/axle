#include <array>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
#include <map>
#include "icr_assert.h"

/*==================================================*/
/*===================== COLORS =====================*/
/*==================================================*/
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define ORANGE  "\033[38;5;214m" /* Orange */
#define RED     "\033[31m"      /* Red */
#define RESET   "\033[0m"

enum color {
    eGREEN,
    eYELLOW,
    eORANGE,
    eRED,
    eRESET
};

struct DebugLoggerOpt {
    enum debug_mode {
        eOUT,
        eFILE
    };

    debug_mode mode = eOUT;
    std::string filename;
    bool colors = false;
};

class DebugLogger {
private:
    static DebugLogger* instance;
    static std::mutex instanceMutex;
    std::ofstream logFile;
    std::mutex logMutex;
    DebugLoggerOpt opts;
    bool opts_set;

    // Map to associate log levels with colors
    std::map<std::string, std::string> levelColorMap = {
        {"INFO", GREEN},
        {"DEBUG", YELLOW},
        {"WARNING", ORANGE},
        {"ERROR", RED}
    };

    DebugLogger() : opts_set(false) {}

    bool openLogFile() {
        if (!opts.filename.empty()) {
            logFile.open(opts.filename, std::ios::app);
            if (!logFile.is_open()) {
                std::cerr << "Error: Unable to open log file " << opts.filename << std::endl;
                return false;
            }
        }
        return true;
    }

public:
    DebugLogger(const DebugLogger&) = delete;
    DebugLogger& operator=(const DebugLogger&) = delete;

    ~DebugLogger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    static DebugLogger* getInstance() {
        std::lock_guard<std::mutex> lock(instanceMutex);
        if (instance == nullptr) {
            instance = new DebugLogger();
        }
        return instance;
    }

    static void set_opts(const DebugLoggerOpt& options) {
        DebugLogger* dbg = getInstance();
        std::lock_guard<std::mutex> lock(dbg->logMutex);
        dbg->opts = options;
        dbg->opts_set = true;

        if (options.mode == DebugLoggerOpt::eFILE && !dbg->logFile.is_open()) {
            if (!dbg->openLogFile()) {
                dbg->opts.mode = DebugLoggerOpt::eOUT; // Fall back to console if file can't open
            }
        }
    }

    template<typename... Args>
    void log(const std::string& level, Args... args) {
        std::ostringstream stream;
        (stream << ... << args);
        std::string message = level + " : " + stream.str();
        std::lock_guard<std::mutex> lock(logMutex);

        // Apply color based on the log level
        if (opts.colors && levelColorMap.find(level) != levelColorMap.end()) {
            message = levelColorMap[level] + message + RESET;
        }

        if (opts.mode == DebugLoggerOpt::eFILE && logFile.is_open()) {
            logFile << message << std::endl;
        } else {
            std::cout << message << std::endl;
        }
    }
};

DebugLogger* DebugLogger::instance = nullptr;
std::mutex DebugLogger::instanceMutex;

#define LOGL(level, ...) DebugLogger::getInstance()->log(level, __VA_ARGS__)
#define LOG(...) DebugLogger::getInstance()->log(level, __VA_ARGS__)

int main() {
    DebugLoggerOpt options;
    options.mode = DebugLoggerOpt::eOUT;
    options.colors = true;
    DebugLogger::set_opts(options);

    LOGL("INFO", "System initialized with value: ", 42, ", status: ", true);
    LOGL("DEBUG", "User coordinates: ", 10.0, ", ", 20.0);
    LOGL("WARNING", "Memory usage is above threshold: ", 75, "%");
    LOGL("ERROR", "Critical failure in module ", "network");

    XCHECK(3 == 4);

    return 0;
}
