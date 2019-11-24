#include "engine.h"
#include "client_config.h"
#include "input/keyboard.h"
#include "renderer/gl/gl_errors.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <common/types.h>
#include <glad/glad.h>
#include <iostream>

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

    auto handleWindowEvents(sf::Window &window, client::Keyboard &keyboard)
    {
        auto status = client::EngineStatus::Ok;
        sf::Event e;
        while (window.pollEvent(e)) {
            keyboard.update(e);
            switch (e.type) {
                case sf::Event::KeyPressed:
                    switch (e.key.code) {
                        case sf::Keyboard::Escape:
                            status = client::EngineStatus::Exit;
                            break;
                        default:
                            break;
                    }
                    break;

                case sf::Event::KeyReleased:
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

/*
    void printFps(client::FPSCounter counter)
    {
        std::cout << "Frame Time: " << counter.currentMsPerFrame() << " ms\n"
                  << "FPS: " << counter.currentFps() << "\n\n";
    }
*/
} // namespace

namespace client {
    EngineStatus runClientEngine(const Config &config)
    {
        // Setup window
        sf::Window window;
        window.setKeyRepeatEnabled(false);
        if (config.fullScreen) {
            createWindow(window, sf::VideoMode::getDesktopMode(),
                         sf::Style::Fullscreen);
        }
        else {
            auto w = static_cast<unsigned>(config.windowWidth);
            auto h = static_cast<unsigned>(config.windowHeight);
            createWindow(window, {w, h}, sf::Style::Close);
        }
        if (config.isFpsCapped) {
            window.setFramerateLimit(config.fpsLimit);
        }

        // Setup OpenGL
        if (!gladLoadGL()) {
            return EngineStatus::GLInitError;
        }
        auto status = EngineStatus::Ok;
        glClearColor(0.25, 0.75, 1.0, 1.0);
        glViewport(0, 0, window.getSize().x, window.getSize().y);
        glCheck(glEnable(GL_DEPTH_TEST));
        glCheck(glEnable(GL_CULL_FACE));
        glCheck(glCullFace(GL_BACK));

        // Start main loop of the game
        Keyboard keyboard;

        sf::Clock fpsTimer;
        while (status == EngineStatus::Ok) {
            // Input
            status = handleWindowEvents(window, keyboard);

            // Update

            /* do update stuff here */

            // Render
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            window.display();

            //Stats
            
        }
        return status;
    }
} // namespace client