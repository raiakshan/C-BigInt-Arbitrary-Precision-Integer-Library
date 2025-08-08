#include "BigInt.h"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <random>
#include <chrono>
#include <climits>

// Static constants
const BigInt BigInt::ZERO(0);
const BigInt BigInt::ONE(1);
const BigInt BigInt::TWO(2);

// Constructors
BigInt::BigInt() : digits({0}), negative(false) {}

BigInt::BigInt(const std::string& str) : negative(false) {
    if (str.empty()) {
        throw InvalidInputException("Empty string");
    }
    
    size_t start = 0;
    if (str[0] == '-') {
        negative = true;
        start = 1;
    } else if (str[0] == '+') {
        start = 1;
    }
    
    if (start >= str.length()) {
        throw InvalidInputException("Invalid number format");
    }
    
    digits.clear();
    for (size_t i = str.length() - 1; i >= start; --i) {
        if (!std::isdigit(str[i])) {
            throw InvalidInputException("Non-digit character in number");
        }
        digits.push_back(str[i] - '0');
    }
    
    normalize();
}

BigInt::BigInt(long long num) : negative(false) {
    if (num < 0) {
        negative = true;
        num = -num;
    }
    
    if (num == 0) {
        digits = {0};
        return;
    }
    
    digits.clear();
    while (num > 0) {
        digits.push_back(num % 10);
        num /= 10;
    }
}

BigInt::BigInt(const BigInt& other) : digits(other.digits), negative(other.negative) {}

BigInt::BigInt(BigInt&& other) noexcept 
    : digits(std::move(other.digits)), negative(other.negative) {
    other.digits = {0};
    other.negative = false;
}

// Assignment operators
BigInt& BigInt::operator=(const BigInt& other) {
    if (this != &other) {
        digits = other.digits;
        negative = other.negative;
    }
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this != &other) {
        digits = std::move(other.digits);
        negative = other.negative;
        other.digits = {0};
        other.negative = false;
    }
    return *this;
}

BigInt& BigInt::operator=(long long num) {
    *this = BigInt(num);
    return *this;
}

// Helper methods
void BigInt::normalize() {
    removeLeadingZeros();
    if (digits.size() == 1 && digits[0] == 0) {
        negative = false;
    }
}

void BigInt::removeLeadingZeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
}

int BigInt::compareMagnitude(const BigInt& other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() < other.digits.size() ? -1 : 1;
    }
    
    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            return digits[i] < other.digits[i] ? -1 : 1;
        }
    }
    return 0;
}

// Arithmetic operators
BigInt BigInt::operator+(const BigInt& other) const {
    if (negative != other.negative) {
        if (negative) {
            return other - (-*this);
        } else {
            return *this - (-other);
        }
    }
    
    BigInt result;
    result.negative = negative;
    result.digits.clear();
    
    int carry = 0;
    size_t maxSize = std::max(digits.size(), other.digits.size());
    
    for (size_t i = 0; i < maxSize || carry; ++i) {
        int sum = carry;
        if (i < digits.size()) sum += digits[i];
        if (i < other.digits.size()) sum += other.digits[i];
        
        result.digits.push_back(sum % 10);
        carry = sum / 10;
    }
    
    return result;
}

BigInt BigInt::operator-(const BigInt& other) const {
    if (negative != other.negative) {
        if (negative) {
            return -((-*this) + other);
        } else {
            return *this + (-other);
        }
    }
    
    if (negative) {
        return (-other) - (-*this);
    }
    
    if (*this < other) {
        return -(other - *this);
    }
    
    BigInt result;
    result.digits.clear();
    
    int borrow = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
        int diff = digits[i] - borrow;
        if (i < other.digits.size()) {
            diff -= other.digits[i];
        }
        
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        
        result.digits.push_back(diff);
    }
    
    result.normalize();
    return result;
}

BigInt BigInt::operator*(const BigInt& other) const {
    if (isZero() || other.isZero()) {
        return ZERO;
    }
    
    BigInt result = schoolbookMultiply(other);
    result.negative = negative != other.negative;
    result.normalize();
    return result;
}

BigInt BigInt::schoolbookMultiply(const BigInt& other) const {
    BigInt result;
    result.digits.resize(digits.size() + other.digits.size(), 0);
    
    for (size_t i = 0; i < digits.size(); ++i) {
        int carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            long long product = result.digits[i + j] + carry;
            if (j < other.digits.size()) {
                product += (long long)digits[i] * other.digits[j];
            }
            result.digits[i + j] = product % 10;
            carry = product / 10;
        }
    }
    
    result.normalize();
    return result;
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (other.isZero()) {
        throw DivisionByZeroException();
    }
    
    std::pair<BigInt, BigInt> result = divideWithRemainder(*this, other);
    result.first.negative = negative != other.negative;
    result.first.normalize();
    return result.first;
}

BigInt BigInt::operator%(const BigInt& other) const {
    if (other.isZero()) {
        throw DivisionByZeroException();
    }
    
    std::pair<BigInt, BigInt> result = divideWithRemainder(*this, other);
    result.second.negative = negative;
    result.second.normalize();
    return result.second;
}

std::pair<BigInt, BigInt> BigInt::divideWithRemainder(const BigInt& dividend, const BigInt& divisor) {
    if (dividend.isZero()) {
        return {ZERO, ZERO};
    }
    
    BigInt quotient, remainder;
    BigInt current = abs(dividend);
    BigInt divisorAbs = abs(divisor);
    
    for (int i = current.digits.size() - 1; i >= 0; --i) {
        remainder.digits.insert(remainder.digits.begin(), current.digits[i]);
        remainder.normalize();
        
        int digit = 0;
        while (remainder >= divisorAbs) {
            remainder = remainder - divisorAbs;
            digit++;
        }
        
        quotient.digits.insert(quotient.digits.begin(), digit);
    }
    
    quotient.normalize();
    return {quotient, remainder};
}

BigInt BigInt::operator^(const BigInt& other) const {
    return pow(*this, other);
}

// Compound assignment operators
BigInt& BigInt::operator+=(const BigInt& other) {
    *this = *this + other;
    return *this;
}

BigInt& BigInt::operator-=(const BigInt& other) {
    *this = *this - other;
    return *this;
}

BigInt& BigInt::operator*=(const BigInt& other) {
    *this = *this * other;
    return *this;
}

BigInt& BigInt::operator/=(const BigInt& other) {
    *this = *this / other;
    return *this;
}

BigInt& BigInt::operator%=(const BigInt& other) {
    *this = *this % other;
    return *this;
}

BigInt& BigInt::operator^=(const BigInt& other) {
    *this = *this ^ other;
    return *this;
}

// Increment/Decrement operators
BigInt& BigInt::operator++() {
    *this = *this + ONE;
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt temp = *this;
    ++(*this);
    return temp;
}

BigInt& BigInt::operator--() {
    *this = *this - ONE;
    return *this;
}

BigInt BigInt::operator--(int) {
    BigInt temp = *this;
    --(*this);
    return temp;
}

// Comparison operators
bool BigInt::operator==(const BigInt& other) const {
    return negative == other.negative && digits == other.digits;
}

bool BigInt::operator!=(const BigInt& other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
    if (negative != other.negative) {
        return negative;
    }
    
    if (negative) {
        return compareMagnitude(other) > 0;
    } else {
        return compareMagnitude(other) < 0;
    }
}

bool BigInt::operator<=(const BigInt& other) const {
    return *this < other || *this == other;
}

bool BigInt::operator>(const BigInt& other) const {
    return !(*this <= other);
}

bool BigInt::operator>=(const BigInt& other) const {
    return !(*this < other);
}

// Unary operators
BigInt BigInt::operator-() const {
    BigInt result = *this;
    result.negative = !negative;
    return result;
}

BigInt BigInt::operator+() const {
    return *this;
}

// Utility methods
bool BigInt::isZero() const {
    return digits.size() == 1 && digits[0] == 0;
}

bool BigInt::isNegative() const {
    return negative && !isZero();
}

bool BigInt::isPositive() const {
    return !negative && !isZero();
}

int BigInt::getDigitCount() const {
    return digits.size();
}

std::string BigInt::toString() const {
    if (isZero()) {
        return "0";
    }
    
    std::string result;
    if (negative) {
        result += "-";
    }
    
    for (int i = digits.size() - 1; i >= 0; --i) {
        result += '0' + digits[i];
    }
    
    return result;
}

long long BigInt::toLongLong() const {
    if (*this > BigInt(LLONG_MAX) || *this < BigInt(LLONG_MIN)) {
        throw OutOfRangeException("Value too large for long long");
    }
    
    long long result = 0;
    for (int i = digits.size() - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }
    
    return negative ? -result : result;
}

// Mathematical functions
BigInt BigInt::factorial(int n) {
    if (n < 0) {
        throw InvalidInputException("Factorial not defined for negative numbers");
    }
    
    BigInt result(1);
    for (int i = 2; i <= n; ++i) {
        result *= BigInt(i);
    }
    return result;
}

BigInt BigInt::fibonacci(int n) {
    if (n < 0) {
        throw InvalidInputException("Fibonacci not defined for negative numbers");
    }
    
    if (n <= 1) {
        return BigInt(n);
    }
    
    BigInt a(0), b(1);
    for (int i = 2; i <= n; ++i) {
        BigInt temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

BigInt BigInt::catalan(int n) {
    if (n < 0) {
        throw InvalidInputException("Catalan numbers not defined for negative numbers");
    }
    
    BigInt numerator = factorial(2 * n);
    BigInt denominator = factorial(n + 1) * factorial(n);
    return numerator / denominator;
}

BigInt BigInt::gcd(const BigInt& a, const BigInt& b) {
    BigInt x = abs(a);
    BigInt y = abs(b);
    
    while (!y.isZero()) {
        BigInt temp = y;
        y = x % y;
        x = temp;
    }
    return x;
}

BigInt BigInt::lcm(const BigInt& a, const BigInt& b) {
    if (a.isZero() || b.isZero()) {
        return ZERO;
    }
    return abs(a * b) / gcd(a, b);
}

bool BigInt::isPrime(const BigInt& n) {
    if (n <= ONE) {
        return false;
    }
    if (n == TWO) {
        return true;
    }
    if (n % TWO == ZERO) {
        return false;
    }
    
    return isPrimeMillerRabin(n);
}

bool BigInt::isPrimeMillerRabin(const BigInt& n, int iterations) {
    // Simple primality test for demo
    BigInt num = abs(n);
    if (num <= BigInt(100)) {
        // For small numbers, use trial division
        for (BigInt i = TWO; i * i <= num; i = i + ONE) {
            if (num % i == ZERO) {
                return false;
            }
        }
        return true;
    }
    
    // For larger numbers, use a simple probabilistic test
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(2, 100);
    
    for (int i = 0; i < iterations; ++i) {
        BigInt a(dis(gen));
        if (gcd(a, num) != ONE) {
            return false;
        }
    }
    return true;
}

std::vector<std::pair<BigInt, int>> BigInt::primeFactorization(const BigInt& n) {
    std::vector<std::pair<BigInt, int>> factors;
    BigInt num = abs(n);
    
    if (num <= ONE) {
        return factors;
    }
    
    // Check for 2
    int count = 0;
    while (num % TWO == ZERO) {
        num = num / TWO;
        count++;
    }
    if (count > 0) {
        factors.emplace_back(TWO, count);
    }
    
    // Check odd numbers up to sqrt(num)
    BigInt i(3);
    BigInt sqrtNum = sqrt(num);
    
    while (i <= sqrtNum) {
        count = 0;
        while (num % i == ZERO) {
            num = num / i;
            count++;
        }
        if (count > 0) {
            factors.emplace_back(i, count);
        }
        i += TWO;
    }
    
    if (num > ONE) {
        factors.emplace_back(num, 1);
    }
    
    return factors;
}

BigInt BigInt::sqrt(const BigInt& n) {
    if (n < ZERO) {
        throw InvalidInputException("Square root not defined for negative numbers");
    }
    
    if (n <= ONE) {
        return n;
    }
    
    BigInt left(1), right = n;
    BigInt result(1);
    
    while (left <= right) {
        BigInt mid = (left + right) / TWO;
        BigInt square = mid * mid;
        
        if (square <= n) {
            result = mid;
            left = mid + ONE;
        } else {
            right = mid - ONE;
        }
    }
    
    return result;
}

BigInt BigInt::pow(const BigInt& base, const BigInt& exponent) {
    if (exponent < ZERO) {
        throw InvalidInputException("Negative exponent not supported");
    }
    
    if (exponent.isZero()) {
        return ONE;
    }
    
    BigInt result(1);
    BigInt b = base;
    BigInt e = exponent;
    
    while (!e.isZero()) {
        if (e % TWO == ONE) {
            result = result * b;
        }
        b = b * b;
        e = e / TWO;
    }
    
    return result;
}

BigInt BigInt::modPow(const BigInt& base, const BigInt& exponent, const BigInt& modulus) {
    if (modulus <= ZERO) {
        throw InvalidInputException("Modulus must be positive");
    }
    
    if (exponent < ZERO) {
        throw InvalidInputException("Negative exponent not supported");
    }
    
    BigInt result(1);
    BigInt b = base % modulus;
    BigInt e = exponent;
    
    while (!e.isZero()) {
        if (e % TWO == ONE) {
            result = (result * b) % modulus;
        }
        b = (b * b) % modulus;
        e = e / TWO;
    }
    
    return result;
}

// Performance and utility
void BigInt::reserve(size_t size) {
    digits.reserve(size);
}

size_t BigInt::size() const {
    return digits.size();
}

void BigInt::clear() {
    digits = {0};
    negative = false;
}

// I/O operators
std::ostream& operator<<(std::ostream& os, const BigInt& num) {
    os << num.toString();
    return os;
}

std::istream& operator>>(std::istream& is, BigInt& num) {
    std::string str;
    is >> str;
    num = BigInt(str);
    return is;
}

// Non-member functions
BigInt abs(const BigInt& num) {
    return num.isNegative() ? -num : num;
}

BigInt min(const BigInt& a, const BigInt& b) {
    return a < b ? a : b;
}

BigInt max(const BigInt& a, const BigInt& b) {
    return a > b ? a : b;
}

// Literal operator
BigInt operator""_bigint(const char* str, size_t size) {
    return BigInt(std::string(str, size));
}