#include "DebugLogger.h"
#include "ConsoleLoggerHandler.h"
#include "FileLoggerHandler.h"
#include "../dbg/xassert.h"
#include <thread>
#include <iostream>


class BankAccount {
    double balance;

public:
    BankAccount(double initial_balance) : balance(initial_balance) {
        XINVARIANT(balance >= 0);  // Invariant: balance should never be negative
    }

    void Deposit(double amount) {
        XPRECONDITION(amount > 0);  // Precondition: deposit amount must be positive

        balance += amount;

        XPOSTCONDITION(balance >= amount);  // Postcondition: balance should have increased
        XINVARIANT(balance >= 0);           // Invariant check
    }

    void Withdraw(double amount) {
        XPRECONDITION(amount > 0);                // Precondition: withdrawal amount must be positive
        XPRECONDITION(balance >= amount);         // Precondition: balance must be sufficient

        balance -= amount;

        XPOSTCONDITION(balance >= 0);             // Postcondition: balance should not go negative
        XINVARIANT(balance >= 0);                 // Invariant check
    }

    double GetBalance() const {
        XINVARIANT(balance >= 0);                 // Invariant: balance should never be negative
        return balance;
    }
};

void logSampleMessages(const std::string& prefix) {
    LOGL("INFO", prefix, " System initialized with value: ", 42, ", status: ", true);
    LOGL("DEBUG", prefix, " User coordinates: ", 10.0, ", ", 20.0);
    LOGL("WARNING", prefix, " Memory usage is above threshold: ", 75, "%");
    LOGL("ERROR", prefix, " Critical failure in module ", "network");
}
void test_log(){

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

}

void test_asserts()
{
    int x = 5;
    int y = 10;

    // This will print "Check failed" message to std::cerr but won't terminate the program
    XCHECK(x > y);

    // This will also print the failure with a custom message but won't terminate the program
    XCHECK_MSG(x > y, "Expected: ", x, " to be greater than ", y);

    std::cout << "Program continues after check failures.\n";

    //XASSERT(x > y);

}

void test_contracts()
{
    BankAccount account(100);
    account.Deposit(50);
    account.Withdraw(30);
    account.Withdraw(150);  // This will fail the precondition
}

int main() {
    test_log();
    test_asserts();
    test_contracts();
}
