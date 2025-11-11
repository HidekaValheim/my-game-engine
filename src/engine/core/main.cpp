
#include "engine/core/Engine.h"
#include <iostream>

int main(int argc, char* argv[]) {
    engine::Engine engine;

    if (!engine.initialize()) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }

    engine.run();
    engine.shutdown();

    return 0;
}
