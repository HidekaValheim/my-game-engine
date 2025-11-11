#pragma once

#include <string>

namespace engine {

class Engine {
public:
    Engine();
    ~Engine();

    bool initialize();
    void run();
    void shutdown();

private:
    bool m_running;
    std::string m_windowTitle;
    int m_windowWidth;
    int m_windowHeight;
};

}  // namespace engine
