#ifndef BIGINT_H
#define BIGINT_H

#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <stdexcept>

// Exception classes
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

class BigInt {
private:
    std::vector<int> digits;
    bool negative;
    
    // Helper methods
    void normalize();
    void removeLeadingZeros();
    int compareMagnitude(const BigInt& other) const;
    BigInt schoolbookMultiply(const BigInt& other) const;
    static std::pair<BigInt, BigInt> divideWithRemainder(const BigInt& dividend, const BigInt& divisor);
    static bool isPrimeMillerRabin(const BigInt& n, int iterations = 5);
    static BigInt modPow(const BigInt& base, const BigInt& exponent, const BigInt& modulus);

public:
    // Constructors
    BigInt();
    BigInt(const std::string& str);
    BigInt(long long num);
    BigInt(const BigInt& other);
    BigInt(BigInt&& other) noexcept;
    
    // Assignment operators
    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;
    BigInt& operator=(long long num);
    
    // Arithmetic operators
    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;
    BigInt operator%(const BigInt& other) const;
    BigInt operator^(const BigInt& other) const; // Power
    
    // Compound assignment operators
    BigInt& operator+=(const BigInt& other);
    BigInt& operator-=(const BigInt& other);
    BigInt& operator*=(const BigInt& other);
    BigInt& operator/=(const BigInt& other);
    BigInt& operator%=(const BigInt& other);
    BigInt& operator^=(const BigInt& other);
    
    // Increment/Decrement operators
    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator--();
    BigInt operator--(int);
    
    // Comparison operators
    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;
    
    // Unary operators
    BigInt operator-() const;
    BigInt operator+() const;
    
    // Utility methods
    bool isZero() const;
    bool isNegative() const;
    bool isPositive() const;
    int getDigitCount() const;
    std::string toString() const;
    long long toLongLong() const;
    
    // Mathematical functions
    static BigInt factorial(int n);
    static BigInt fibonacci(int n);
    static BigInt catalan(int n);
    static BigInt gcd(const BigInt& a, const BigInt& b);
    static BigInt lcm(const BigInt& a, const BigInt& b);
    static bool isPrime(const BigInt& n);
    static std::vector<std::pair<BigInt, int>> primeFactorization(const BigInt& n);
    static BigInt sqrt(const BigInt& n);
    static BigInt pow(const BigInt& base, const BigInt& exponent);
    
    // Performance and utility
    void reserve(size_t size);
    size_t size() const;
    void clear();
    
    // Friend functions for I/O
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
    friend std::istream& operator>>(std::istream& is, BigInt& num);
    
    // Constants
    static const BigInt ZERO;
    static const BigInt ONE;
    static const BigInt TWO;
};

// Non-member functions
BigInt abs(const BigInt& num);
BigInt min(const BigInt& a, const BigInt& b);
BigInt max(const BigInt& a, const BigInt& b);

// Literal operator for BigInt
BigInt operator""_bigint(const char* str, size_t size);

#endif // BIGINT_H