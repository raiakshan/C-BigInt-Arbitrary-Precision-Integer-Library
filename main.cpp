#include "BigInt.h"
#include <iostream>
#include <chrono>
#include <iomanip>

void printHeader(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << " " << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

void basicOperations() {
    printHeader("BASIC ARITHMETIC OPERATIONS");
    
    BigInt a(123456789);
    BigInt b(987654321);
    
    std::cout << "a = " << a << "\n";
    std::cout << "b = " << b << "\n";
    std::cout << "a + b = " << a + b << "\n";
    std::cout << "a - b = " << a - b << "\n";
    std::cout << "a * b = " << a * b << "\n";
    std::cout << "b / a = " << b / a << "\n";
    std::cout << "b % a = " << b % a << "\n";
}

void mathematicalFunctions() {
    printHeader("MATHEMATICAL FUNCTIONS");
    
    std::cout << "Factorial of 15 = " << BigInt::factorial(15) << "\n";
    std::cout << "Fibonacci(30) = " << BigInt::fibonacci(30) << "\n";
    std::cout << "Catalan(8) = " << BigInt::catalan(8) << "\n";
    
    BigInt a(48), b(18);
    std::cout << "GCD(" << a << ", " << b << ") = " << BigInt::gcd(a, b) << "\n";
    std::cout << "LCM(" << a << ", " << b << ") = " << BigInt::lcm(a, b) << "\n";
}

void advancedFeatures() {
    printHeader("ADVANCED FEATURES");
    
    BigInt num(100);
    std::cout << "Square root of " << num << " = " << BigInt::sqrt(num) << "\n";
    
    BigInt prime(17);
    std::cout << prime << " is " << (BigInt::isPrime(prime) ? "prime" : "not prime") << "\n";
    
    BigInt composite(100);
    std::cout << composite << " is " << (BigInt::isPrime(composite) ? "prime" : "not prime") << "\n";
}

void performanceDemo() {
    printHeader("PERFORMANCE DEMONSTRATION");
    
    auto start = std::chrono::high_resolution_clock::now();
    BigInt fact = BigInt::factorial(50);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Factorial(50) calculated in " << duration.count() << "ms\n";
    std::cout << "Result has " << fact.getDigitCount() << " digits\n";
}

int main() {
    std::cout << "Professional BigInt Library Demo\n";
    std::cout << "================================\n";
    
    try {
        basicOperations();
        mathematicalFunctions();
        advancedFeatures();
        performanceDemo();
        
        std::cout << "\n" << std::string(60, '=') << "\n";
        std::cout << " All operations completed successfully!\n";
        std::cout << std::string(60, '=') << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}