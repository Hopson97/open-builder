#include "client_engine.h"

#include "window.h"
#include "gl/gl_errors.h"
#include <glad/glad.h>
#include "gameplay.h"

namespace {
struct FPSCounter {

};
}

EngineStatus runClientEngine(const ClientConfig &config)
{
    // Create the window
    Window window(config);

    // Setup OpenGL
    if (!gladLoadGL()) {
        return EngineStatus::GLInitError;
    }

    glClearColor(0.25, 0.75, 1.0, 1.0);
    glViewport(0, 0, window.width, window.height);

    glCheck(glEnable(GL_DEPTH_TEST));
    glCheck(glEnable(GL_CULL_FACE));
    glCheck(glCullFace(GL_BACK));

    Gameplay gameplay;
    gameplay.init(window.aspect);

    Keyboard keyboard;
    EngineStatus status = EngineStatus::Ok;
    sf::Clock frameTimer;
    int frameCount = 0;
    while (status == EngineStatus::Ok) {
        status = window.pollEvents(keyboard, [](auto key) { (void)key; }
        );
        gameplay.handleInput(window.window, keyboard);
        gameplay.update();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gameplay.render();
        window.window.display();

        // Stats
        /*
        frameCount++;
        if (frameTimer.getElapsedTime().asSeconds() > 2) {
            float ms = static_cast<float>(
                frameTimer.getElapsedTime().asMilliseconds());
            float secs = frameTimer.getElapsedTime().asSeconds();
            std::cout << "Average Frame Time: " << ms / frameCount
                      << "Average FPS: " << frameCount / secs << "\n\n";
            frameCount = 0;
            frameTimer.restart();
        }
        */
    }
    gameplay.endGame();
    return status;
}
