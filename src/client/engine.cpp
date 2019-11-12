#include "engine.h"

#include <iostream>

#include "renderer/gl/gl_errors.h"
#include <SFML/Window/Event.hpp>
#include <glad/glad.h>

#include "game_states/survival_state.h"
#include <common/launch_config.h>

namespace client {
    Engine::Engine(const LaunchConfig &config)
        : m_camera(config)
    {
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.antialiasingLevel = 4;
        settings.majorVersion = 3;
        settings.minorVersion = 3;

        if (config.graphicOptions.fullScreen) {
            m_window.create(sf::VideoMode::getDesktopMode(), "open-builder",
                            sf::Style::Fullscreen, settings);
        }
        else {
            const unsigned WINDOW_WIDTH = config.graphicOptions.windowWidth;
            const unsigned WINDOW_HEIGHT = config.graphicOptions.windowHeight;

            m_window.create({WINDOW_WIDTH, WINDOW_HEIGHT}, "open-builder",
                            sf::Style::Close, settings);
        }

        if (config.graphicOptions.isFpsCapped) {
            m_window.setFramerateLimit(config.graphicOptions.fpsLimit);
        }

        if (!gladLoadGL()) {
            m_status = Status::GLInitError;
        }
        else {
            m_status = Status::Ok;
            glClearColor(0.25, 0.75, 1.0, 1.0);
            glViewport(0, 0, m_window.getSize().x, m_window.getSize().y);
            m_stateHandler.pushState<SurvivalState>(m_stateHandler);
            m_renderer = std::make_unique<Renderer>();
        }
    }

    Engine::Status Engine::runClient()
    {
        m_fpsCounter.reset();
        int frame = 0;
        while (m_status == Status::Ok) {
            mp_currentState = &m_stateHandler.peekState();

            handleWindowEvents();

            handleInput();
            update();
            render();

            m_fpsCounter.update();

            if (frame++ == 256) {
                std::cout << "Frame Time: " << m_fpsCounter.currentMsPerFrame()
                          << " ms"
                          << '\n'
                          //<< "FPS: " << m_fpsCounter.currentFps()
                          << std::endl;
                frame = 0;
            }

            m_stateHandler.update();
            if (m_stateHandler.isEmpty()) {
                exit();
            }
        }
        m_stateHandler.clear();

        return m_status;
    }

    void Engine::handleInput()
    {
        mp_currentState->handleInput(m_keyboard, m_window);
    }

    void Engine::update()
    {
        mp_currentState->update(m_camera);
    }

    void Engine::render()
    {
        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mp_currentState->render(*m_renderer);
        m_renderer->render(m_camera);
        m_window.display();
    }

    void Engine::handleWindowEvents()
    {
        sf::Event e;
        while (m_window.pollEvent(e)) {
            m_keyboard.update(e);
            switch (e.type) {
                case sf::Event::KeyPressed:
                    mp_currentState->handleKeyDown(e.key.code);
                    switch (e.key.code) {
                        case sf::Keyboard::Escape:
                            exit();
                            break;
                        default:
                            break;
                    }
                    break;

                case sf::Event::KeyReleased:
                    mp_currentState->handleKeyUp(e.key.code);
                    break;

                case sf::Event::Closed:
                    exit();
                    break;

                default:
                    break;
            }
        }
    }

    void Engine::exit()
    {
        m_status = Status::Exit;
    }

} // namespace client