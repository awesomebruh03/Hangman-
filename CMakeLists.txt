cmake_minimum_required(VERSION 3.10)

# Project Name
project(HangmanGame VERSION 1.0 LANGUAGES CXX)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(HangmanGame hello.cpp)

# Find and link nlohmann-json installed via pacman
find_package(nlohmann_json REQUIRED)

# Link the library to the project
target_link_libraries(HangmanGame PRIVATE nlohmann_json::nlohmann_json)

# Optional: Add extra warning flags for development
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(HangmanGame PRIVATE -Wall -Wextra -pedantic)
endif()
