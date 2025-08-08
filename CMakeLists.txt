cmake_minimum_required(VERSION 3.15)
project(BigInt VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3")
endif()

# Include directories
include_directories(include)

# Find required packages
find_package(GTest QUIET)

# Source files
set(SOURCES
    src/BigInt.cpp
    src/BigIntExceptions.cpp
)

set(HEADERS
    include/BigInt.h
    include/BigIntExceptions.h
)

# Create library
add_library(BigInt ${SOURCES} ${HEADERS})
target_include_directories(BigInt PUBLIC include)

# Main executable
add_executable(bigint_demo src/main.cpp)
target_link_libraries(bigint_demo BigInt)

# Tests
if(GTest_FOUND)
    enable_testing()
    add_executable(bigint_tests tests/test_BigInt.cpp)
    target_link_libraries(bigint_tests BigInt GTest::GTest GTest::Main)
    add_test(NAME BigIntTests COMMAND bigint_tests)
endif()

# Benchmarks
add_executable(bigint_benchmarks benchmarks/benchmarks.cpp)
target_link_libraries(bigint_benchmarks BigInt)

# Installation
install(TARGETS BigInt
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
install(FILES ${HEADERS} DESTINATION include/BigInt)

# Package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/BigIntConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/BigIntConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/BigIntConfig.cmake"
    INSTALL_DESTINATION lib/cmake/BigInt
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/BigIntConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/BigIntConfigVersion.cmake"
    DESTINATION lib/cmake/BigInt
)