cmake_minimum_required(VERSION 3.20)
project(MyGameEngine LANGUAGES CXX)

# --- C++ Standard ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Build Type ---
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# --- Subdirectories ---
add_subdirectory(src)
add_subdirectory(third_party)

# --- Optional: Enable testing ---
# enable_testing()
# add_subdirectory(tests)
