add_executable(engine
    engine/core/main.cpp
    engine/core/Engine.cpp
    engine/render/Shader.cpp
    engine/math/Math.cpp
)

target_include_directories(engine PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(engine PRIVATE
    SDL2::SDL2
    SDL2::SDL2main
    glad::glad
)