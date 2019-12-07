#include "client_engine.h"
#include "client_config.h"
#include "gl/gl_errors.h"
#include "gl/gl_object.h"
#include "input/keyboard.h"
#include "maths.h"
#include "network/client.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <common/debug.h>
#include <common/network/packet.h>
#include <common/types.h>
#include <glad/glad.h>
#include <iostream>

namespace {

class Window {
  public:
    Window(const ClientConfig &config)
    {
        window.setKeyRepeatEnabled(false);
        if (config.fullScreen) {
            create(sf::VideoMode::getDesktopMode(), sf::Style::Fullscreen);
            width = window.getSize().x;
            height = window.getSize().y;
        }
        else {
            width = static_cast<unsigned>(config.windowWidth);
            height = static_cast<unsigned>(config.windowHeight);
            create({width, height}, sf::Style::Close);
        }
        if (config.isFpsCapped) {
            window.setFramerateLimit(config.fpsLimit);
        }
    }

    auto pollEvents(Keyboard &keyboard)
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

    sf::Window window;
    unsigned width;
    unsigned height;

  private:
    void create(const sf::VideoMode &mode, u32 style)
    {
        sf::ContextSettings settings;
        settings.depthBits = 24;
        settings.stencilBits = 8;
        settings.antialiasingLevel = 4;
        settings.majorVersion = 3;
        settings.minorVersion = 3;

        window.create(mode, "Open Builder", style, settings);
    }
};

gl::VertexArray createCube()
{
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

    gl::VertexArray vao;
    vao.create();
    vao.bind();
    vao.addVertexBuffer(3, vertices);
    vao.addVertexBuffer(2, textureCoords);
    vao.addIndexBuffer(indices);

    return vao;
}

} // namespace

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

    auto cube = createCube();

    gl::Shader shader;
    shader.create("static", "static");
    shader.bind();
    auto mdLocarion = shader.getUniformLocation("modelMatrix");
    auto pvLocation = shader.getUniformLocation("projectionViewMatrix");

    gl::Texture2d texture;
    texture.create("grass");
    texture.bind();

    glm::mat4 modelMatrix{1.0f};
    glm::mat4 projectionMatrix{1.0f};
    glm::mat4 projectionViewMatrix{1.0f};

    projectionMatrix =
        glm::perspective(3.14f / 2.0f, 1280.0f / 720.0f, 0.01f, 100.0f);

    ClientConnection client;
    client.connectTo(sf::IpAddress::LocalHost);
    auto &player = client.players[client.getClientId()];

    // Start main loop of the game
    Keyboard keyboard;
    sf::Clock frameTimer;
    int frameCount = 0;
    auto lastMousepositionition = sf::Mouse::getPosition(window.window);
    auto status = EngineStatus::Ok;
    bool useMouse = true;
    while (status == EngineStatus::Ok) {
        // Input
        status = window.pollEvents(keyboard);

        if (useMouse) {

            // Mouse input
            auto change =
                sf::Mouse::getPosition(window.window) - lastMousepositionition;

            player.rotation.x += static_cast<float>(change.y / 2.0f);
            player.rotation.y += static_cast<float>(change.x / 2.0f);

            lastMousepositionition = sf::Mouse::getPosition(window.window);

            player.rotation.x = glm::clamp(player.rotation.x, -170.0f, 170.0f);
            // sf::Mouse::setPosition({(int)window.width / 2, (int)window.height
            // / 2});
        }

        if (keyboard.keyReleased(sf::Keyboard::L)) {
            useMouse = !useMouse;
        }

        const float PLAYER_SPEED = 0.05f;
        float rads = (glm::radians(player.rotation.y));
        float rads90 = (glm::radians(player.rotation.y + 90));
        if (keyboard.isKeyDown(sf::Keyboard::W)) {
            player.position.x -= glm::cos(rads90) * PLAYER_SPEED;
            player.position.z -= glm::sin(rads90) * PLAYER_SPEED;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::S)) {
            player.position.x += glm::cos(rads90) * PLAYER_SPEED;
            player.position.z += glm::sin(rads90) * PLAYER_SPEED;
        }
        if (keyboard.isKeyDown(sf::Keyboard::A)) {
            player.position.x -= glm::cos(rads) * PLAYER_SPEED;
            player.position.z -= glm::sin(rads) * PLAYER_SPEED;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::D)) {
            player.position.x += glm::cos(rads) * PLAYER_SPEED;
            player.position.z += glm::sin(rads) * PLAYER_SPEED;
        }

        // Update
        glm::mat4 viewMatrix{1.0f};
        rotateMatrix(&viewMatrix, player.rotation);
        translateMatrix(&viewMatrix, -player.position);
        projectionViewMatrix = projectionMatrix * viewMatrix;

        gl::loadUniform(pvLocation, projectionViewMatrix);
        gl::loadUniform(mdLocarion, modelMatrix);

        client.sendPlayerPosition();

        Packet incoming;
        while (receivePacket(client.m_socket, incoming)) {
            switch (static_cast<ClientCommand>(incoming.command)) {
                case ClientCommand::ConnectionChallenge:
                    break;

                case ClientCommand::AcceptConnection:
                    break;

                case ClientCommand::RejectConnection:
                    break;

                case ClientCommand::PlayerJoin:
                    LOG("Player joined");
                    break;

                case ClientCommand::PlayerLeave:
                    LOG("Player left");
                    break;

                case ClientCommand::Snapshot: {
                    u16 n = 0;
                    incoming.data >> n;

                    for (u16 i = 0; i < n; i++) {
                        client_id_t id = 0;
                        float x, y, z;
                        incoming.data >> id >> x >> y >> z;
                        if (id != client.getClientId()) {
                            auto *p = &client.players[id];
                            p->position = {x, y, z};
                        }
                    }

                } break;

                default:
                    break;
            }
        }

        // Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (auto &p : client.players) {
            modelMatrix = glm::mat4{1.0f};
            translateMatrix(&modelMatrix,
                            {p.position.x, p.position.y, p.position.z});
            gl::loadUniform(mdLocarion, modelMatrix);
            cube.getDrawable().drawElements();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }

        window.window.display();

        // Stats
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
    }

    cube.destroy();
    texture.destroy();
    shader.destroy();

    client.disconnect();

    return status;
}
