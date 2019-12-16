#include "client_engine.h"

#include "../gameplay.h"
#include "../gl/gl_errors.h"
#include "window.h"
#include <common/debug.h>
#include <glad/glad.h>
#include <iostream>

namespace {
struct FPSCounter final {
    float frameCount = 0;
    sf::Clock timer;

    void update()
    {
        frameCount++;
        if (timer.getElapsedTime() > sf::seconds(2)) {
            auto time = timer.getElapsedTime();

            float fps = frameCount / time.asSeconds();
            float frameTime = time.asMilliseconds() / frameCount;

            std::cout << "==============\nAverage Current Performance\nFPS: "
                      << fps << "\nFrame time: " << frameTime << "ms\n\n";
            frameCount = 0;
            timer.restart();
        }
    }
};
} // namespace

EngineStatus runClientEngine(const ClientConfig &config)
{
    // Create the window
    Window window(config);
    window.window.setMouseCursorVisible(false);

    // Setup OpenGL
    if (!gladLoadGL()) {
        return EngineStatus::GLInitError;
    }

    glClearColor(0.25, 0.75, 1.0, 1.0);
    glViewport(0, 0, window.width, window.height);

    glCheck(glEnable(GL_DEPTH_TEST));
    glCheck(glEnable(GL_CULL_FACE));
    glCheck(glCullFace(GL_BACK));

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Gameplay gameplay;
    Keyboard keyboard;
    EngineStatus status = EngineStatus::Ok;
    FPSCounter counter;

    if (!gameplay.init(window.aspect)) {
        return EngineStatus::CouldNotConnect;
    }

    LOG("Client", "Starting game.");
    while (status == EngineStatus::Ok) {
        // Input
        status = window.pollEvents(
            keyboard, [&gameplay](auto key) { gameplay.onKeyRelease(key); });

        gameplay.handleInput(window.window, keyboard);

        // Update
        gameplay.update();

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gameplay.render();
        window.window.display();

        // Stats and stuff
        counter.update();
        if (status == EngineStatus::Ok) {
            status = gameplay.currentStatus();
        }
    }
    window.window.close();
    gameplay.endGame();
    return status;
}
