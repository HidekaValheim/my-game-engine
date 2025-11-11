#pragma once

typedef struct SDL_Window SDL_Window;
typedef void* SDL_GLContext;

#include "engine/render/Shader.h"
#include "engine/math/Math.h"

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
        void update(float dt);
        void render();

        SDL_Window* m_window = nullptr;
        SDL_GLContext m_glContext = nullptr;
        bool m_running = false;

        const char* m_title = "My Game Engine";
        int m_width = 800;
        int m_height = 600;

        Shader m_shader;
        unsigned int m_vao = 0, m_vbo = 0;
        float m_rotation = 0.0f;

        double m_lastTime = 0.0;
        double m_accumulator = 0.0;
        const double m_fixedTimestep = 1.0 / 60.0;
    };

}  // namespace engine