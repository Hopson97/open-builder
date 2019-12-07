#include "client_engine.h"
#include "client_config.h"
#include "gl/gl_errors.h"
#include "gl/gl_object.h"
#include "input/keyboard.h"
#include "maths.h"
#include "network/client.h"
#include "window.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>
#include <common/debug.h>
#include <common/network/packet.h>
#include <common/types.h>
#include <glad/glad.h>
#include <iostream>

namespace {
gl::VertexArray createCube()
{
    // Create a cube for opengl testing
    std::vector<GLfloat> vertices = {0, 0, 2, 2, 0, 2, 2, 2, 2, 0, 2, 2,
                                     // right
                                     2, 0, 0, 2, 2, 0, 2, 2, 2, 2, 0, 2,
                                     // back
                                     0, 0, 0, 0, 2, 0, 2, 2, 0, 2, 0, 0,
                                     // left
                                     0, 0, 0, 0, 0, 2, 0, 2, 2, 0, 2, 0,
                                     // bottom
                                     0, 0, 2, 0, 0, 0, 2, 0, 0, 2, 0, 2,
                                     // top
                                     0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 2, 0};
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

struct Entity {
    glm::vec3 position{0.0f, 0.0f, -12.0f}, rotation{0.0f};
    bool active = false;
};

class ClientEngine {
  private:
    Window &m_window;
    Keyboard m_keyboard;

    gl::VertexArray m_cube;
    gl::Shader m_shader;
    gl::Texture2d m_texture;

    gl::UniformLocation m_modelLocation;
    gl::UniformLocation m_projectionViewLocation;

    ClientConnection m_client;
    std::array<Entity, 512> m_entities;

    Entity *m_player = nullptr;

  public:
    ClientEngine(Window &window)
        : m_window(window)
    {
    }

    ~ClientEngine()
    {
        m_cube.destroy();
        m_texture.destroy();
        m_shader.destroy();

        m_client.disconnect();
    }

    EngineStatus run()
    {
        init();

        glm::mat4 projectionMatrix{1.0f};
        projectionMatrix =
            glm::perspective(3.14f / 2.0f, 1280.0f / 720.0f, 0.01f, 100.0f);

        EngineStatus status = EngineStatus::Ok;
        sf::Clock frameTimer;
        int frameCount = 0;
        while (status == EngineStatus::Ok) {
            // Input
            status = m_window.pollEvents(m_keyboard);
            input();

            // Update
            update();

            // Draw
            render(projectionMatrix);

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
        return status;
    }

  private:
    void input()
    {
        static auto lastMousepositionition =
            sf::Mouse::getPosition(m_window.window);

        auto change =
            sf::Mouse::getPosition(m_window.window) - lastMousepositionition;

        m_player->rotation.x += static_cast<float>(change.y / 2.0f);
        m_player->rotation.y += static_cast<float>(change.x / 2.0f);

        lastMousepositionition = sf::Mouse::getPosition(m_window.window);

        const float PLAYER_SPEED = 0.05f;
        float rads = (glm::radians(m_player->rotation.y));
        float rads90 = (glm::radians(m_player->rotation.y + 90));
        if (m_keyboard.isKeyDown(sf::Keyboard::W)) {
            m_player->position.x -= glm::cos(rads90) * PLAYER_SPEED;
            m_player->position.z -= glm::sin(rads90) * PLAYER_SPEED;
        }
        else if (m_keyboard.isKeyDown(sf::Keyboard::S)) {
            m_player->position.x += glm::cos(rads90) * PLAYER_SPEED;
            m_player->position.z += glm::sin(rads90) * PLAYER_SPEED;
        }
        if (m_keyboard.isKeyDown(sf::Keyboard::A)) {
            m_player->position.x -= glm::cos(rads) * PLAYER_SPEED;
            m_player->position.z -= glm::sin(rads) * PLAYER_SPEED;
        }
        else if (m_keyboard.isKeyDown(sf::Keyboard::D)) {
            m_player->position.x += glm::cos(rads) * PLAYER_SPEED;
            m_player->position.z += glm::sin(rads) * PLAYER_SPEED;
        }
    }

    void update()
    {
        auto packet = makePacket(ServerCommand::PlayerPosition);
        auto id = m_client.getClientId();
        packet.data << id << m_entities[id].position.x
                    << m_entities[id].position.y << m_entities[id].position.z;
        m_client.sendPacketToServer(packet);
        handlePackets();
    }

    void render(const glm::mat4 &projectionMatrix)
    {
        glm::mat4 viewMatrix{1.0f};
        rotateMatrix(&viewMatrix, m_player->rotation);
        translateMatrix(&viewMatrix, -m_player->position);

        glm::mat4 projectionViewMatrix{1.0f};
        projectionViewMatrix = projectionMatrix * viewMatrix;
        gl::loadUniform(m_projectionViewLocation, projectionViewMatrix);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto drawable = m_cube.getDrawable();
        drawable.bind();
        for (auto &p : m_entities) {
            if (p.active) {

                glm::mat4 modelMatrix{1.0f};
                translateMatrix(&modelMatrix,
                                {p.position.x, p.position.y, p.position.z});
                gl::loadUniform(m_modelLocation, modelMatrix);
                drawable.draw();
            }
        }

        glm::mat4 modelMatrix{1.0f};
        gl::loadUniform(m_modelLocation, modelMatrix);
        drawable.draw();
        m_window.window.display();
    }

    void init()
    {
        m_cube = createCube();

        m_shader.create("static", "static");
        m_shader.bind();
        m_modelLocation = m_shader.getUniformLocation("modelMatrix");
        m_projectionViewLocation =
            m_shader.getUniformLocation("projectionViewMatrix");

        m_texture.create("grass");
        m_texture.bind();

        m_client.connectTo(sf::IpAddress::LocalHost);
        m_player = &m_entities[m_client.getClientId()];
    }

    void handlePackets()
    {
        Packet incoming;
        while (receivePacket(m_client.socket, incoming)) {
            switch (static_cast<ClientCommand>(incoming.command)) {
                case ClientCommand::PlayerJoin:
                    handlePlayerJoin(incoming);
                    break;

                case ClientCommand::PlayerLeave:
                    handlePlayerLeave(incoming);
                    break;

                case ClientCommand::Snapshot:
                    handleSnapshot(incoming);
                    break;

                case ClientCommand::ConnectionChallenge:
                case ClientCommand::AcceptConnection:
                case ClientCommand::RejectConnection:
                    break;
            }
        }
    }

    void handleSnapshot(Packet &packet)
    {
        u16 n = 0;
        packet.data >> n;

        for (u16 i = 0; i < n; i++) {
            client_id_t id = 0;
            float x, y, z;
            packet.data >> id >> x >> y >> z;
            if (id != m_client.getClientId()) {
                auto *p = &m_entities[id];
                p->position = {x, y, z};
            }
        }
    }

    void handlePlayerJoin(Packet &packet)
    {
        client_id_t id = 0;
        packet.data >> id;
        m_entities[id].active = true;
    }

    void handlePlayerLeave(Packet &packet)
    {
        client_id_t id = 0;
        packet.data >> id;
        m_entities[id].active = false;
    }
};

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

    ClientEngine engine(window);
    return engine.run();
}
