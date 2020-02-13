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

    int updates = 0;

    float frameTime = 0;

    sf::Time totalTime;
    float totalFrames = 0;

    void update()
    {
        frameCount++;
        if (timer.getElapsedTime() > sf::seconds(0.25)) {
            auto time = timer.getElapsedTime();

            frameTime = time.asMilliseconds() / frameCount;

            if (updates++ > 20) {
                totalFrames += frameCount;
                totalTime += timer.getElapsedTime();
            }
            timer.restart();
            frameCount = 0;
        }
    }
};
} // namespace

EngineStatus runClientEngine(const ClientConfig& config)
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
#ifndef __APPLE__
    initGLDebug();
#endif

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Client gameClient(config);
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
        gameClient.update(clock.restart().asSeconds(), counter.frameTime);

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
