#include "engine.h"

#include <iostream>

#include "renderer/gl/gl_errors.h"
#include <SFML/Window/Event.hpp>
#include <glad/glad.h>

#include "game_states/survival_state.h"
#include <common/launch_config.h>

namespace {
    void createWindow(sf::Window &window, const sf::VideoMode &mode, u32 style)
    {
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.antialiasingLevel = 4;
        settings.majorVersion = 3;
        settings.minorVersion = 3;

        window.create(mode, "Open Builder", style, settings);
    }

    auto handleWindowEvents(sf::Window &window, client::Keyboard &keyboard,
                            client::GameState &currentState)
    {
        auto status = client::EngineStatus::Ok;
        sf::Event e;
        while (window.pollEvent(e)) {
            keyboard.update(e);
            switch (e.type) {
                case sf::Event::KeyPressed:
                    currentState.handleKeyDown(e.key.code);
                    switch (e.key.code) {
                        case sf::Keyboard::Escape:
                            status = client::EngineStatus::Exit;
                            break;
                        default:
                            break;
                    }
                    break;

                case sf::Event::KeyReleased:
                    currentState.handleKeyUp(e.key.code);
                    break;

                case sf::Event::Closed:
                    status = client::EngineStatus::Exit;
                    break;

                default:
                    break;
            }
        }
        return status;
    }

    void printFps(client::FPSCounter counter)
    {
        std::cout << "Frame Time: " << counter.currentMsPerFrame() << " ms\n"
                  << "FPS: " << counter.currentFps() << "\n\n";
    }
} // namespace

namespace client {
    EngineStatus runClientEngine(const LaunchConfig &config)
    {
        // Setup window
        sf::Window window;
        window.setKeyRepeatEnabled(false);
        if (config.graphicOptions.fullScreen) {
            createWindow(window, sf::VideoMode::getDesktopMode(),
                         sf::Style::Fullscreen);
        }
        else {
            auto w = static_cast<unsigned>(config.graphicOptions.windowWidth);
            auto h = static_cast<unsigned>(config.graphicOptions.windowHeight);
            createWindow(window, {w, h}, sf::Style::Close);
        }
        if (config.graphicOptions.isFpsCapped) {
            window.setFramerateLimit(config.graphicOptions.fpsLimit);
        }

        // Setup OpenGL
        if (!gladLoadGL()) {
            return EngineStatus::GLInitError;
        }
        auto status = EngineStatus::Ok;
        glClearColor(0.25, 0.75, 1.0, 1.0);
        glViewport(0, 0, window.getSize().x, window.getSize().y);

        // Start main loop of the game
        Camera camera(config);
        Keyboard keyboard;
        Renderer renderer;
        StateHandler stateHandler;
        GameState *p_currentState = nullptr;
        stateHandler.pushState<SurvivalState>(stateHandler);

        sf::Clock fpsTimer;
        FPSCounter fps;
        while (status == EngineStatus::Ok) {
            p_currentState = &stateHandler.peekState();

            status = handleWindowEvents(window, keyboard, *p_currentState);

            p_currentState->handleInput(keyboard, window);
            p_currentState->update(camera);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            p_currentState->render(renderer);
            renderer.render(camera);
            window.display();

            fps.update();
            if (fpsTimer.getElapsedTime() > sf::seconds(4.0f)) {
                printFps(fps);
            }

            stateHandler.update();
            if (stateHandler.isEmpty()) {
                status = EngineStatus::Exit;
            }
        }
        return status;
    }
} // namespace client