#pragma once

// SDL2 opaque types - use void* (MSVC requires this)
typedef struct SDL_Window SDL_Window;
typedef void* SDL_GLContext;

namespace engine {

class Engine {
public:
    Engine();
    ~Engine();

    bool initialize();
    void run();
    void shutdown();

private:
    void pollEvents();

    SDL_Window* m_window = nullptr;
    SDL_GLContext m_glContext = nullptr;
    bool m_running = false;

    const char* m_title = "My Game Engine";
    int m_width = 800;
    int m_height = 600;
};

}  // namespace engine
