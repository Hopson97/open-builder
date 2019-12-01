#include "client_engine.h"
#include "client_config.h"
#include "gl/gl_errors.h"
#include "gl/gl_object.h"
#include "input/keyboard.h"
#include "maths.h"
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
        auto width = static_cast<unsigned>(config.windowWidth);
        auto height = static_cast<unsigned>(config.windowHeight);
        createWindow(window, {width, height}, sf::Style::Close);
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

struct {
    glm::vec3 pos{0.0, 0.0, 12.0f}, rot;
} player;

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

    // Create a cube for opengl testing
    std::vector<GLfloat> vertices = {0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1,
                                     // right
                                     1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1,
                                     // back
                                     0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0,
                                     // left
                                     0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
                                     // bottom
                                     0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1,
                                     // top
                                     0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0};
    std::vector<GLuint> indices;
    for (int itr = 0, i = 0; itr < 6; itr++) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
        indices.push_back(i + 2);
        indices.push_back(i + 3);
        indices.push_back(i);
        i += 4;
    }
    std::vector<GLfloat> textureCoords;

    for (int i = 0; i < 6; i++) {
        textureCoords.insert(textureCoords.end(),
                             {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f});
    }

    gl::Object<gl::VertexArray> vao;
    vao->create();
    vao.bind();
    vao->addVertexBuffer(3, vertices);
    vao->addVertexBuffer(2, textureCoords);
    vao->addIndexBuffer(indices);

    gl::Object<gl::Shader> shader;
    shader.create("static", "static");
    shader.bind();
    auto mdLocarion = shader->getUniformLocation("modelMatrix");
    auto pvLocation = shader->getUniformLocation("projectionViewMatrix");

    gl::Object<gl::Texture2d> texture;
    texture.create("grass");
    texture.bind();

    glm::mat4 modelMatrix{1.0f};
    glm::mat4 projectionMatrix{1.0f};
    glm::mat4 projectionViewMatrix{1.0f};

    projectionMatrix =
        glm::perspective(3.14f / 2.0f, 1280.0f / 720.0f, 0.01f, 100.0f);
    modelMatrix = glm::rotate(modelMatrix, 3.14f / 4.0f, {1.0, 0.0, 0.0});

    // Start main loop of the game
    Keyboard keyboard;
    sf::Clock frameTimer;
    int frameCount = 0;
    auto lastMousePosition = sf::Mouse::getPosition(window);
    while (status == EngineStatus::Ok) {
        // Input
        status = handleWindowEvents(window, keyboard);

        // Mouse input
        auto change = sf::Mouse::getPosition(window) - lastMousePosition;

        player.rot.x += static_cast<float>(change.y / 16.0f);
        player.rot.y += static_cast<float>(change.x / 16.0f);

        lastMousePosition = sf::Mouse::getPosition(window);

        player.rot.x = glm::clamp(player.rot.x, -170.0f, 170.0f);

		const float PLAYER_SPEED = 0.05f;
		float rads = (glm::radians(player.rot.y));
		float rads90 = (glm::radians(player.rot.y + 90));
        if (keyboard.isKeyDown(sf::Keyboard::W)) {
			player.pos.x -= glm::cos(rads90) * PLAYER_SPEED;
            player.pos.z -= glm::sin(rads90) * PLAYER_SPEED;
		}
        else if (keyboard.isKeyDown(sf::Keyboard::S)) {
            player.pos.x += glm::cos(rads90) * PLAYER_SPEED;
            player.pos.z += glm::sin(rads90) * PLAYER_SPEED;
        }
        if (keyboard.isKeyDown(sf::Keyboard::A)) {
            player.pos.x -= glm::cos(rads) * PLAYER_SPEED;
            player.pos.z -= glm::sin(rads) * PLAYER_SPEED;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::D)) {
            player.pos.x += glm::cos(rads) * PLAYER_SPEED;
            player.pos.z += glm::sin(rads) * PLAYER_SPEED;
        }

            // Update
            glm::mat4 viewMatrix{1.0f};
        rotateMatrix(&viewMatrix, player.rot);
        translateMatrix(&viewMatrix, -player.pos);
        projectionViewMatrix = projectionMatrix * viewMatrix;

        gl::loadUniform(pvLocation, projectionViewMatrix);
        gl::loadUniform(mdLocarion, modelMatrix);

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        vao->getDrawable().drawElements();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

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
    return status;
}
