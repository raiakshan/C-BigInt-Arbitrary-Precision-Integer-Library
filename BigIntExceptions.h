#ifndef BIGINT_EXCEPTIONS_H
#define BIGINT_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace BigInt {

class DivisionByZeroException : public std::runtime_error {
public:
    explicit DivisionByZeroException(const std::string& message = "Division by zero")
        : std::runtime_error(message) {}
};

class OverflowException : public std::overflow_error {
public:
    explicit OverflowException(const std::string& message = "Arithmetic overflow")
        : std::overflow_error(message) {}
};

class UnderflowException : public std::underflow_error {
public:
    explicit UnderflowException(const std::string& message = "Arithmetic underflow")
        : std::underflow_error(message) {}
};

class InvalidInputException : public std::invalid_argument {
public:
    explicit InvalidInputException(const std::string& message = "Invalid input")
        : std::invalid_argument(message) {}
};

class OutOfRangeException : public std::out_of_range {
public:
    explicit OutOfRangeException(const std::string& message = "Value out of range")
        : std::out_of_range(message) {}
};

} // namespace BigInt

#endif // BIGINT_EXCEPTIONS_H