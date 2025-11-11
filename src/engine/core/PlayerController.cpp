// src/engine/core/PlayerController.cpp
#include "engine/core/PlayerController.h"
#include <iostream>

namespace engine {

    PlayerController::PlayerController() = default;

    void PlayerController::update(float dt, float cameraPos[3], float& yaw, float& pitch, bool& mouseCaptured) {
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        float moveSpeed = 5.0f * dt;

        // Forward/right vectors from yaw/pitch
        float yawRad = yaw * 3.14159f / 180.0f;
        float pitchRad = pitch * 3.14159f / 180.0f;

        float forwardX = sinf(yawRad) * cosf(pitchRad);
        float forwardY = sinf(pitchRad);
        float forwardZ = -cosf(yawRad) * cosf(pitchRad);

        float rightX = cosf(yawRad);
        float rightZ = sinf(yawRad);

        // WASD movement
        if (keys[SDL_SCANCODE_W]) {
            cameraPos[0] += forwardX * moveSpeed;
            cameraPos[1] += forwardY * moveSpeed;
            cameraPos[2] += forwardZ * moveSpeed;
        }
        if (keys[SDL_SCANCODE_S]) {
            cameraPos[0] -= forwardX * moveSpeed;
            cameraPos[1] -= forwardY * moveSpeed;
            cameraPos[2] -= forwardZ * moveSpeed;
        }
        if (keys[SDL_SCANCODE_A]) {
            cameraPos[0] -= rightX * moveSpeed;
            cameraPos[2] -= rightZ * moveSpeed;
        }
        if (keys[SDL_SCANCODE_D]) {
            cameraPos[0] += rightX * moveSpeed;
            cameraPos[2] += rightZ * moveSpeed;
        }

        // Mouse look
        if (m_rightMouseDown) {
            if (!mouseCaptured) {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                mouseCaptured = true;
            }

            int dx, dy;
            SDL_GetRelativeMouseState(&dx, &dy);
            yaw += dx * 0.22f;
            pitch -= dy * 0.22f;

            if (pitch > 89.0f) pitch = 89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }
        else {
            if (mouseCaptured) {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                mouseCaptured = false;
            }
        }
    }

    void PlayerController::handleEvent(const SDL_Event& event) {
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
            m_rightMouseDown = true;
        }
        if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT) {
            m_rightMouseDown = false;
        }
    }

} // namespace engine