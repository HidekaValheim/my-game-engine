#include "engine/core/Engine.h"
#include <SDL.h>
#include <glad/glad.h>
#include "engine/math/Math.h"
#include <iostream>

namespace engine {

    Engine::Engine() = default;

    Engine::~Engine() {
        shutdown();
    }

    bool Engine::initialize() {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        m_window = SDL_CreateWindow(
            m_title,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            m_width, m_height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );

        if (!m_window) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            return false;
        }

        m_glContext = SDL_GL_CreateContext(m_window);
        if (!m_glContext) {
            std::cerr << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
            return false;
        }

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return false;
        }

        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.4f, 0.6f, 1.0f, 1.0f);

        if (!m_shader.loadFromFile("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl")) {
            std::cerr << "Failed to load shaders!" << std::endl;
            return false;
        }

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
             0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
        };

        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m_lastTime = SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
        m_running = true;
        return true;
    }

    void Engine::pollEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) m_running = false;
        }
    }

    void Engine::update(float dt) {
        m_rotation += 90.0f * dt;
    }

    void Engine::render() {
        m_shader.reloadIfNeeded();
        m_shader.bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        using namespace math;
        Mat4 proj = perspective(45.0f * 3.14159f / 180.0f, (float)m_width / m_height, 0.1f, 100.0f);
        Mat4 view = lookAt(Vec3(0, 0, 3), Vec3(0, 0, 0), Vec3(0, 1, 0));
        Mat4 model = rotate(Mat4(), m_rotation * 3.14159f / 180.0f, Vec3(0, 0, 1));
        Mat4 mvp; mul(mvp.m, proj.m, view.m);
        Mat4 temp; mul(temp.m, mvp.m, model.m);
        m_shader.setMat4("uMVP", temp.m);

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        m_shader.unbind();
    }

    void Engine::run() {
        while (m_running) {
            double currentTime = SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
            double frameTime = currentTime - m_lastTime;
            m_lastTime = currentTime;
            m_accumulator += frameTime;

            pollEvents();

            while (m_accumulator >= m_fixedTimestep) {
                update((float)m_fixedTimestep);
                m_accumulator -= m_fixedTimestep;
            }

            render();
            SDL_GL_SwapWindow(m_window);
        }
    }

    void Engine::shutdown() {
        if (m_glContext) SDL_GL_DeleteContext(m_glContext);
        if (m_window) SDL_DestroyWindow(m_window);
        SDL_Quit();
        if (m_vao) glDeleteVertexArrays(1, &m_vao);
        if (m_vbo) glDeleteBuffers(1, &m_vbo);
    }

}  // namespace engine