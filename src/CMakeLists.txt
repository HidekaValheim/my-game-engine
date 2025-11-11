add_executable(engine
    engine/core/main.cpp
    engine/core/Engine.cpp
)

target_include_directories(engine PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
