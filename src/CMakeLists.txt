add_executable(engine
    engine/core/main.cpp
)

target_include_directories(engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# We'll link SDL2, GLFW, etc. later — for now, just build
