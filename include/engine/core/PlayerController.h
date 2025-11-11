// include/engine/core/PlayerController.h
#pragma once
#include <SDL.h>

namespace engine {

    class PlayerController {
    public:
        PlayerController();
        void update(float dt, float cameraPos[3], float& yaw, float& pitch, bool& mouseCaptured);
        void handleEvent(const SDL_Event& event);

    private:
        bool m_rightMouseDown = false;
    };

} // namespace engine