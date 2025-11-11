cmake_minimum_required(VERSION 3.20)
project(MyGameEngine LANGUAGES CXX)

# vcpkg toolchain
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{HOME}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

# THIS LINE IS THE FIX:
add_subdirectory(src "${CMAKE_CURRENT_BINARY_DIR}/src")