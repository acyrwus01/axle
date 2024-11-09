#ifndef XASSERT_LIB_H
#define XASSERT_LIB_H

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cmath>

// Macro to enable or disable assertions
#define ENABLE_XASSERTIONS 1

#if ENABLE_XASSERTIONS

// Custom exception for assertion failures
class AssertionFailed : public std::runtime_error {
public:
    explicit AssertionFailed(const std::string& message)
        : std::runtime_error(message) {}
};

// Helper function to format message with multiple arguments
template <typename... Args>
std::string FormatMessage(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << args);  // Fold expression to concatenate all arguments
    return oss.str();
}

// Base assertion function template with formatted message support
template <typename T>
void XAssertImpl(bool condition, const T& expression, const char* exprStr, const char* file, int line, const std::string& customMessage = "") {
    if (!condition) {
        std::ostringstream oss;
        oss << "Assertion failed: (" << exprStr << ") in file " << file << ", line " << line;
        if (!customMessage.empty()) {
            oss << ": " << customMessage;
        }
        std::cerr << oss.str() << "\n";
        throw AssertionFailed(oss.str());
    }
}

// Base check function template for non-throwing checks
template <typename T>
void XCheckImpl(bool condition, const T& expression, const char* exprStr, const char* file, int line, const std::string& customMessage = "") {
    if (!condition) {
        std::cerr << "Check failed: (" << exprStr << ") in file " << file << ", line " << line;
        if (!customMessage.empty()) {
            std::cerr << ": " << customMessage;
        }
        std::cerr << "\n";
    }
}

// XASSERT macros for runtime checks with exceptions
#define XASSERT(expr) XAssertImpl((expr), (expr), #expr, __FILE__, __LINE__)
#define XASSERT_MSG(expr, ...) XAssertImpl((expr), (expr), #expr, __FILE__, __LINE__, FormatMessage(__VA_ARGS__))

// XCHECK macros for runtime checks without exceptions
#define XCHECK(expr) XCheckImpl((expr), (expr), #expr, __FILE__, __LINE__)
#define XCHECK_MSG(expr, ...) XCheckImpl((expr), (expr), #expr, __FILE__, __LINE__, FormatMessage(__VA_ARGS__))

// Floating-point comparison with tolerance
#define XASSERT_FLOAT_EQ(lhs, rhs, tol) XASSERT_MSG(std::fabs((lhs) - (rhs)) <= (tol), "Expected: " #lhs " == " #rhs " within tolerance " #tol)
#define XASSERT_FLOAT_NE(lhs, rhs, tol) XASSERT_MSG(std::fabs((lhs) - (rhs)) > (tol), "Expected: " #lhs " != " #rhs " beyond tolerance " #tol)

// Pointer null checks
#define XASSERT_NOT_NULL(ptr) XASSERT_MSG((ptr) != nullptr, "Expected: " #ptr " is not NULL")
#define XASSERT_NULL(ptr) XASSERT_MSG((ptr) == nullptr, "Expected: " #ptr " is NULL")

// Array size check
#define XASSERT_ARRAY_SIZE(arr, expected_size) XASSERT_MSG((sizeof(arr) / sizeof(arr[0])) == (expected_size), "Expected array size: " #expected_size)

// Compile-time static assert for compile-time validation
#define XSTATIC_ASSERT(cond, msg) static_assert((cond), msg)

#else
// If assertions are disabled, define empty macros
#define XASSERT(expr) ((void)0)
#define XASSERT_MSG(expr, ...) ((void)0)
#define XCHECK(expr) ((void)0)
#define XCHECK_MSG(expr, ...) ((void)0)
#define XASSERT_FLOAT_EQ(lhs, rhs, tol) ((void)0)
#define XASSERT_FLOAT_NE(lhs, rhs, tol) ((void)0)
#define XASSERT_NOT_NULL(ptr) ((void)0)
#define XASSERT_NULL(ptr) ((void)0)
#define XASSERT_ARRAY_SIZE(arr, expected_size) ((void)0)
#define XSTATIC_ASSERT(cond, msg) ((void)0)

#endif // ENABLE_XASSERTIONS

#endif // XASSERT_LIB_H
