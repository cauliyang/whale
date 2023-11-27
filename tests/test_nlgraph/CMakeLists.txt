# Minimum required version of CMake
cmake_minimum_required(VERSION 3.25)

# Project name and language
project(WhaleTestsNLGraph LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Prepare doctest for other targets to use
find_package(doctest REQUIRED)

# Create the test executable
add_executable(test_nlgraph main.cpp)

# If you are using a custom library that needs to be linked:
target_link_libraries(test_nlgraph doctest::doctest)

# Discover tests
add_test(NAME WhaleTestsNLGraph COMMAND test_nlgraph)
