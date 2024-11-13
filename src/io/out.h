#ifndef OUT_H_
#define OUT_H_

#include <sstream>
#include <string>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace {
    struct Formatter {
    private:
        template <typename T>
        static void AppendToStream(std::ostringstream& oss, T&& arg) {
            oss << std::forward<T>(arg);
        }

        template <typename First, typename... Rest>
        static void AppendToStream(std::ostringstream& oss, First&& first, Rest&&... rest) {
            oss << std::forward<First>(first);
            AppendToStream(oss, std::forward<Rest>(rest)...);
        }

    public:
        template <typename... Args>
        static std::string FormatMessage(Args&&... args) {
            std::ostringstream oss;
            AppendToStream(oss, std::forward<Args>(args)...);
            return oss.str();
        }

        template <typename... Args>
        static void print(Args&&... args) {
            std::cout << FormatMessage(std::forward<Args>(args)...);
        }

        template <typename... Args>
        static void println(Args&&... args) {
            std::cout << FormatMessage(std::forward<Args>(args)...) << std::endl;
        }
    };
}

namespace io
{
    // Public API functions
    template <typename... Args>
    std::string FormatMessage(Args&&... args) {
        return Formatter::FormatMessage(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void print(Args&&... args) {
        Formatter::print(std::forward<Args>(args)...);
    }

    template <typename... Args>
    void println(Args&&... args) {
        Formatter::println(std::forward<Args>(args)...);
    }
}
#endif // OUT_H_
