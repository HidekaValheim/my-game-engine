// src/engine/core/Engine.cpp
#include "engine/core/Engine.h"
#include <SDL.h>
#include <glad/glad.h>
#include "engine/math/Math.h"
#include <iostream>
#include <filesystem>
#include <windows.h>

namespace engine {

    Engine::Engine() = default;

    Engine::~Engine() {
        shutdown();
    }

    bool Engine::initialize() {
        std::cout << "GROK ENGINE STARTING...\n";

        // GROK CWD FIX
        char exePath[MAX_PATH];
        if (GetModuleFileNameA(NULL, exePath, MAX_PATH) != 0) {
            std::filesystem::path p = std::filesystem::path(exePath).parent_path();
            for (int i = 0; i < 10; ++i) {
                if (std::filesystem::exists(p / "assets" / "shaders" / "vertex.glsl")) {
                    std::filesystem::current_path(p);
                    std::cout << "[GROK] CWD FIXED TO: " << std::filesystem::current_path() << "\n";
                    break;
                }
                p = p.parent_path();
            }
        }

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return false;
        }

        // FORCE MODERN OPENGL + DEBUG + NO COMPATIBILITY CRAP
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG | SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

        m_window = SDL_CreateWindow(
            "GROK ENGINE — SPINNING RAINBOW TRIANGLE",
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
        // ENABLE OPENGL DEBUG OUTPUT (ONLY WORKS ON 4.3+)
        if (GLAD_GL_VERSION_4_3) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
                if (severity <= GL_DEBUG_SEVERITY_MEDIUM) {
                    std::cerr << "OPENGL ERROR: " << message << std::endl;
                }
                }, nullptr);
            std::cout << "OPENGL DEBUG OUTPUT ENABLED\n";
        }
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // PURE BLACK

        // LOAD SHADERS — WILL STOP IF FAIL
        if (!m_shader.loadFromFile("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl")) {
            std::cerr << "\nFATAL: SHADERS FAILED TO LOAD OR COMPILE!\n";
            std::cerr << "Check console above for GL errors.\n\n";
            system("pause");
            return false;
        }
        std::cout << "SHADERS LOADED AND LINKED SUCCESSFULLY!\n";

        float vertices[] = {
            -1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom left — RED
             1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right — RED
             0.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f   // top — RED
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

        glBindVertexArray(0);

        m_lastTime = SDL_GetPerformanceCounter() / (double)SDL_GetPerformanceFrequency();
        m_running = true;
        std::cout << "GROK ENGINE READY — TRIANGLE WILL SPIN!\n";
        return true;
    }

    void Engine::pollEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) m_running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) m_running = false;
        }
    }
    
    void Engine::update(float dt) {
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        float moveSpeed = 5.0f * dt;
        float rotSpeed = 90.0f * dt;

        // Toggle mouse capture with RIGHT CLICK
        if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
            if (!m_mouseCaptured) {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                m_mouseCaptured = true;
            }
        }
        else {
            if (m_mouseCaptured) {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                m_mouseCaptured = false;
            }
        }

        // Mouse look
        if (m_mouseCaptured) {
            int dx, dy;
            SDL_GetRelativeMouseState(&dx, &dy);
            m_cameraYaw -= dx * 0.2f;
            m_cameraPitch -= dy * 0.2f;
            if (m_cameraPitch > 89.0f) m_cameraPitch = 89.0f;
            if (m_cameraPitch < -89.0f) m_cameraPitch = -89.0f;
        }

        // WASD movement in camera direction
        float forward[3] = { 0 };
        float right[3] = { 0 };
        forward[0] = sinf(m_cameraYaw * 3.14159f / 180.0f);
        forward[2] = -cosf(m_cameraYaw * 3.14159f / 180.0f);
        right[0] = cosf(m_cameraYaw * 3.14159f / 180.0f);
        right[2] = sinf(m_cameraYaw * 3.14159f / 180.0f);

        if (keys[SDL_SCANCODE_W]) {
            m_cameraPos[0] += forward[0] * moveSpeed;
            m_cameraPos[2] += forward[2] * moveSpeed;
        }
        if (keys[SDL_SCANCODE_S]) {
            m_cameraPos[0] -= forward[0] * moveSpeed;
            m_cameraPos[2] -= forward[2] * moveSpeed;
        }
        if (keys[SDL_SCANCODE_A]) {
            m_cameraPos[0] -= right[0] * moveSpeed;
            m_cameraPos[2] -= right[2] * moveSpeed;
        }
        if (keys[SDL_SCANCODE_D]) {
            m_cameraPos[0] += right[0] * moveSpeed;
            m_cameraPos[2] += right[2] * moveSpeed;
        }

        // Spin the triangle too (because it's beautiful)
        m_rotation += 60.0f * dt;
    }
    void Engine::render() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        m_shader.bind();

        // CAMERA DIRECTION (raw floats)
        float yaw = m_cameraYaw * 3.14159f / 180.0f;
        float pitch = m_cameraPitch * 3.14159f / 180.0f;
        float fx = sinf(yaw) * cosf(pitch);
        float fy = sinf(pitch);
        float fz = -cosf(yaw) * cosf(pitch);

        // PROJECTION
        float proj[16];
        float fov = 45.0f * 3.14159f / 180.0f;
        float aspect = (float)m_width / m_height;
        float f = 1.0f / tanf(fov * 0.5f);
        std::memset(proj, 0, sizeof(proj));
        proj[0] = f / aspect;
        proj[5] = f;
        proj[10] = (100.0f + 0.1f) / (0.1f - 100.0f);
        proj[11] = -1.0f;
        proj[14] = (2.0f * 100.0f * 0.1f) / (0.1f - 100.0f);

        // VIEW MATRIX
        float eye[3] = { m_cameraPos[0], m_cameraPos[1], m_cameraPos[2] };
        float target[3] = { eye[0] + fx, eye[1] + fy, eye[2] + fz };
        float up[3] = { 0.0f, 1.0f, 0.0f };

        float view[16];
        mul(view, proj, nullptr); // dummy to reuse mul — we'll build view manually
        {
            float f[3] = { target[0] - eye[0], target[1] - eye[1], target[2] - eye[2] };
            float len, s[3], u[3];
            len = sqrtf(f[0] * f[0] + f[1] * f[1] + f[2] * f[2]);
            if (len > 0.0f) { f[0] /= len; f[1] /= len; f[2] /= len; }

            s[0] = f[1] * up[2] - f[2] * up[1];
            s[1] = f[2] * up[0] - f[0] * up[2];
            s[2] = f[0] * up[1] - f[1] * up[0];
            len = sqrtf(s[0] * s[0] + s[1] * s[1] + s[2] * s[2]);
            if (len > 0.0f) { s[0] /= len; s[1] /= len; s[2] /= len; }

            u[0] = s[1] * f[2] - s[2] * f[1];
            u[1] = s[2] * f[0] - s[0] * f[2];
            u[2] = s[0] * f[1] - s[1] * f[0];

            std::memset(view, 0, sizeof(view));
            view[0] = s[0]; view[1] = u[0]; view[2] = -f[0];
            view[4] = s[1]; view[5] = u[1]; view[6] = -f[1];
            view[8] = s[2]; view[9] = u[2]; view[10] = -f[2];
            view[12] = -(s[0] * eye[0] + s[1] * eye[1] + s[2] * eye[2]);
            view[13] = -(u[0] * eye[0] + u[1] * eye[1] + u[2] * eye[2]);
            view[14] = f[0] * eye[0] + f[1] * eye[1] + f[2] * eye[2];
            view[15] = 1.0f;
        }

        // SPINNING MODEL
        float angle = (float)SDL_GetTicks() / 1000.0f * 90.0f;
        float c = cosf(angle * 3.14159f / 180.0f);
        float s = sinf(angle * 3.14159f / 180.0f);
        float model[16] = {
            c, -s, 0, 0,
            s,  c, 0, 0,
            0,  0, 1, 0,
            0,  0, 0, 1
        };

        // FINAL MVP
        float temp[16];
        mul(temp, view, model);
        float mvp[16];
        mul(mvp, proj, temp);

        m_shader.setMat4("uMVP", mvp);
        m_shader.setFloat("uTime", (float)SDL_GetTicks() / 1000.0f);

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

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