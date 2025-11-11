// include/engine/core/Engine.h
#pragma once

#include <SDL.h>
#include <glad/glad.h>
#include "engine/render/Shader.h"
#include "engine/math/Math.h"

typedef struct SDL_Window SDL_Window;
typedef void* SDL_GLContext;

// Forward declaration — keeps compile times fast and clean
namespace engine {
    class PlayerController;
}

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

        // === WINDOW & CONTEXT ===
        SDL_Window* m_window = nullptr;
        SDL_GLContext m_glContext = nullptr;
        bool m_running = false;

        const char* m_title = "My Game Engine — IT’S SPINNING GROK";
        int m_width = 800;
        int m_height = 600;

        // === RENDERING ===
        Shader m_shader;
        unsigned int m_vao = 0;
        unsigned int m_vbo = 0;

        // === CAMERA & PLAYER ===
        float m_cameraPos[3] = { 0.0f, 0.0f, 5.0f };
        float m_cameraYaw = 0.0f;
        float m_cameraPitch = 0.0f;
        bool m_mouseCaptured = false;

        PlayerController* m_playerController = nullptr;

        // === TRIANGLE ANIMATION ===
        float m_rotation = 0.0f;

        // === TIMING ===
        double m_lastTime = 0.0;
        double m_accumulator = 0.0;
        const double m_fixedTimestep = 1.0 / 60.0;
    };

} // namespace engine