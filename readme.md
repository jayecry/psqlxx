# psqlxx

![C++](https://img.shields.io/badge/language-C++-brightgreen.svg)
![CMake](https://img.shields.io/badge/build-CMake-blue.svg)
![Dockerfile](https://img.shields.io/badge/container-Dockerfile-lightgrey.svg)

`psqlxx` is a modern C++ wrapper for libpq, making it easy to work with PostgreSQL from C++ code.

## Features

- Easy to use
- Modern C++ approach
- High performance
- Support for asynchronous operations

## Installation

### FetchContent

To include the `psqlxx` library in your project using CMake, you can use the FetchContent module:

```cmake
cmake_minimum_required(VERSION 3.16)
project(MyProject LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    psqlxx
    GIT_REPOSITORY "https://github.com/jayecry/psqlxx.git"
    GIT_TAG        "v1.0.0"
)

FetchContent_MakeAvailable(psqlxx)

add_executable(my_app src/main.cpp)

target_link_libraries(my_app PRIVATE PSQLXX::psqlxx_static)
