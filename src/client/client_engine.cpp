#include "client_engine.h"

#include "client.h"
#include "gl/gl_errors.h"
#include "window.h"
#include <SFML/System/Clock.hpp>
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

            std::cout << "FPS: " << fps << "\nFrame time: " << frameTime
                      << "ms\n\n";
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

    glClearColor(0.25f, 0.75f, 1.0f, 1.0f);
    // glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glViewport(0, 0, window.width, window.height);

    glCheck(glEnable(GL_DEPTH_TEST));
    glCheck(glEnable(GL_CULL_FACE));
    glCheck(glCullFace(GL_BACK));
    glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    initGLDebug();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Client gameClient;
    Keyboard keyboard;
    EngineStatus status = EngineStatus::Ok;
    FPSCounter counter;

    if (!gameClient.init(config, window.aspect)) {
        return EngineStatus::CouldNotConnect;
    }

    LOG("Client", "Starting game.");
    sf::Clock clock;
    while (status == EngineStatus::Ok) {
        // Input
        status = window.pollEvents(
            keyboard, [&gameClient](auto key) { gameClient.onKeyRelease(key); },
            [&gameClient](auto button, int x, int y) {
                gameClient.onMouseRelease(button, x, y);
            });

        gameClient.handleInput(window.window, keyboard);

        // Update
        gameClient.update(clock.restart().asSeconds());

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gameClient.render(window.width, window.height);
        window.window.display();

        // Stats and stuff
        counter.update();
        if (status == EngineStatus::Ok) {
            status = gameClient.currentStatus();
        }
    }
    window.window.close();
    gameClient.endGame();
    return status;
}
