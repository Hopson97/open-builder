#include "client_engine.h"
#include "client_config.h"
#include "gl/gl_shader.h"
#include "gl/gl_vertex_array.h"
#include "input/keyboard.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <common/types.h>
#include <glad/glad.h>
#include <iostream>

namespace {
void createWindow(sf::Window *window, const sf::VideoMode &mode, u32 style)
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    window->create(mode, "Open Builder", style, settings);
}

void initWindow(sf::Window *window, const ClientConfig &config)
{
    window->setKeyRepeatEnabled(false);
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
        window->setFramerateLimit(config.fpsLimit);
    }
}

auto handleWindowEvents(sf::Window &window, Keyboard &keyboard)
{
    auto status = EngineStatus::Ok;
    sf::Event e;
    while (window.pollEvent(e)) {

        keyboard.update(e);
        if (e.type == sf::Event::KeyPressed) {
            if (e.key.code == sf::Keyboard::Escape) {
                status = EngineStatus::Exit;
            }
        }
        else if (e.type == sf::Event::Closed) {
            status = EngineStatus::Exit;
        }
    }
    return status;
}
} // namespace

EngineStatus runClientEngine(const ClientConfig &config)
{
    // Create the window
    sf::Window window;
    initWindow(&window, config);

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

    // Create a rectangle for opengl testing
    std::vector<GLfloat> vertices = {0,   0,   0, 0,   0.5, 0,
                                     0.5, 0.5, 0, 0.5, 0,   0};
    std::vector<GLuint> indices = {0, 1, 2, 2, 3, 0};

    auto vao = createVertexArray();
    vao.addVertexBuffer(3, vertices, DrawStyle::Static, GLType::Float);
    vao.addIndexBuffer(indices);

    // Start main loop of the game
    Keyboard keyboard;
    sf::Clock frameTimer;
    int frameCount = 0;
    while (status == EngineStatus::Ok) {
        // Input
        status = handleWindowEvents(window, keyboard);

        // Update

        /* do update stuff here */

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        bindVertexArray(vao.object);
        drawElements(vao.object, vao.indicesCount);

        window.display();

        // Stats
        frameCount++;
        if (frameTimer.getElapsedTime().asSeconds() > 2) {
            float ms = frameTimer.getElapsedTime().asMilliseconds();
            int secs = frameTimer.getElapsedTime().asSeconds();
            std::cout << "Average Frame Time: " << ms / frameCount
                      << "Average FPS: " << frameCount / secs << "\n\n";
            frameCount = 0;
            frameTimer.restart();
        }
    }
    destroyVertexArray(&vao);
    return status;
}
