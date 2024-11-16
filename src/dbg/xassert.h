#ifndef XASSERT_LIB_H
#define XASSERT_LIB_H

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>  // For std::abort
#include <cmath>

// Macro to enable or disable assertions
#define ENABLE_XASSERTIONS 1

#if ENABLE_XASSERTIONS

// Helper function to format message with multiple arguments
template <typename... Args>
std::string FormatMessage(Args&&... args) {
    std::ostringstream oss;
    (oss << ... << args);  // Fold expression to concatenate all arguments
    return oss.str();
}

// Base assertion function without a custom message
inline void XAssertImpl(bool condition, const char* exprStr, const char* file, int line) {
    if (!condition) {
        std::cerr << "Assertion failed: (" << exprStr << ") in file " << file << ", line " << line << "\n";
        std::abort();
    }
}

// Base assertion function with a custom message
inline void XAssertImpl(bool condition, const char* exprStr, const char* file, int line, const std::string& customMessage) {
    if (!condition) {
        std::cerr << "Assertion failed: (" << exprStr << ") in file " << file << ", line " << line << ": " << customMessage << "\n";
        std::abort();
    }
}

// XASSERT macros for runtime checks with termination
#define XASSERT(expr) \
    XAssertImpl((expr), #expr, __FILE__, __LINE__)
#define XASSERT_MSG(expr, ...) \
    XAssertImpl((expr), #expr, __FILE__, __LINE__, FormatMessage(__VA_ARGS__))

// XCHECK macros for runtime checks without termination
#define XCHECK(expr) \
    do { \
        if (!(expr)) { \
            std::cerr << "Check failed: (" << #expr << ") in file " << __FILE__ << ", line " << __LINE__ << "\n"; \
        } \
    } while (0)
#define XCHECK_MSG(expr, ...) \
    do { \
        if (!(expr)) { \
            std::cerr << "Check failed: (" << #expr << ") in file " << __FILE__ << ", line " << __LINE__ << ": " << FormatMessage(__VA_ARGS__) << "\n"; \
        } \
    } while (0)

// XCHECK_ACTION macro for runtime checks with customizable actions
#define XCHECK_ACTION(expr, action) \
    do { \
        if (!(expr)) { \
            std::cerr << "Check failed: (" << #expr << ") in file " << __FILE__ << ", line " << __LINE__ << "\n"; \
            action; \
        } \
    } while (0)

// Design by Contract Macros
// Preconditions: Checks conditions at the start of a function
#define XPRECONDITION(expr) XASSERT(expr)
#define XPRECONDITION_MSG(expr, ...) XASSERT_MSG(expr, FormatMessage("Precondition failed: ", __VA_ARGS__))

// Postconditions: Checks conditions at the end of a function
#define XPOSTCONDITION(expr) XASSERT(expr)
#define XPOSTCONDITION_MSG(expr, ...) XASSERT_MSG(expr, FormatMessage("Postcondition failed: ", __VA_ARGS__))

// Invariants: Checks conditions that must always hold true for the object state
#define XINVARIANT(expr) XCHECK(expr)
#define XINVARIANT_MSG(expr, ...) XCHECK_MSG(expr, FormatMessage("Invariant failed: ", __VA_ARGS__))

// Floating-point comparison with tolerance
#define XASSERT_FLOAT_EQ(lhs, rhs, tol) \
    XASSERT_MSG(std::fabs((lhs) - (rhs)) <= (tol), "Expected: " #lhs " == " #rhs " within tolerance " #tol)
#define XASSERT_FLOAT_NE(lhs, rhs, tol) \
    XASSERT_MSG(std::fabs((lhs) - (rhs)) > (tol), "Expected: " #lhs " != " #rhs " beyond tolerance " #tol)

// Pointer null checks
#define XASSERT_NOT_NULL(ptr) XASSERT_MSG((ptr) != nullptr, "Expected: " #ptr " is not NULL")
#define XASSERT_NULL(ptr) XASSERT_MSG((ptr) == nullptr, "Expected: " #ptr " is NULL")

// Array size check
#define XASSERT_ARRAY_SIZE(arr, expected_size) \
    XASSERT_MSG((sizeof(arr) / sizeof(arr[0])) == (expected_size), "Expected array size: " #expected_size)

// Compile-time static assert for compile-time validation
#define XSTATIC_ASSERT(cond, msg) static_assert((cond), msg)

#else
// If assertions are disabled, define empty macros
#define XASSERT(expr) ((void)0)
#define XASSERT_MSG(expr, ...) ((void)0)
#define XCHECK(expr) ((void)0)
#define XCHECK_MSG(expr, ...) ((void)0)
#define XCHECK_ACTION(expr, action) ((void)0)
#define XPRECONDITION(expr) ((void)0)
#define XPRECONDITION_MSG(expr, ...) ((void)0)
#define XPOSTCONDITION(expr) ((void)0)
#define XPOSTCONDITION_MSG(expr, ...) ((void)0)
#define XINVARIANT(expr) ((void)0)
#define XINVARIANT_MSG(expr, ...) ((void)0)
#define XASSERT_FLOAT_EQ(lhs, rhs, tol) ((void)0)
#define XASSERT_FLOAT_NE(lhs, rhs, tol) ((void)0)
#define XASSERT_NOT_NULL(ptr) ((void)0)
#define XASSERT_NULL(ptr) ((void)0)
#define XASSERT_ARRAY_SIZE(arr, expected_size) ((void)0)
#define XSTATIC_ASSERT(cond, msg) ((void)0)

#endif // ENABLE_XASSERTIONS

#endif // XASSERT_LIB_H
