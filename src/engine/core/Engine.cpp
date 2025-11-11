#include "engine/core/Engine.h"
#include <iostream>

namespace engine {

Engine::Engine()
    : m_running(false)
    , m_windowTitle("My Game Engine")
    , m_windowWidth(800)
    , m_windowHeight(600)
{
    std::cout << "Engine constructed.\n";
}

Engine::~Engine() {
    std::cout << "Engine destroyed.\n";
}

bool Engine::initialize() {
    std::cout << "Initializing engine...\n";
    m_running = true;
    return true;
}

void Engine::run() {
    std::cout << "Engine running (stub loop)...\n";
    // TODO: Main game loop
    while (m_running) {
        // Placeholder: exit after one frame
        m_running = false;
    }
}

void Engine::shutdown() {
    std::cout << "Shutting down engine...\n";
}

}  // namespace engine
