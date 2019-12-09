#include "gameplay.h"

#include <SFML/Window/Mouse.hpp>
#include "input/keyboard.h"
#include <common/network/packet.h>
#include <common/debug.h>

namespace {
gl::VertexArray createCube()
{
    // Create a cube for opengl testing
    std::vector<GLfloat> vertices = {// Front
                                     0, 0, 2, 2, 0, 2, 2, 2, 2, 0, 2, 2,
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
}

void Gameplay::init(float aspect)
{
    m_cube = createCube();

    m_shader.create("static", "static");
    m_shader.bind();
    m_modelLocation = m_shader.getUniformLocation("modelMatrix");
    m_projectionViewLocation =
        m_shader.getUniformLocation("projectionViewMatrix");

    m_texture.create("p2");
    m_texture.bind();

    m_client.connectTo(sf::IpAddress::LocalHost);
    m_player = &m_entities[m_client.getClientId()];

    m_projectionMatrix = glm::perspective(3.14f / 2.0f, aspect, 0.01f, 100.0f);
}

void Gameplay::handleInput(const sf::Window& window, const Keyboard &keyboard)
{
    static auto lastMousepositionition =
        sf::Mouse::getPosition(window);

    if (!m_isMouseLocked && window.hasFocus()) {
        auto change =
            sf::Mouse::getPosition(window) - lastMousepositionition;
        m_player->rotation.x += static_cast<float>(change.y / 8.0f);
        m_player->rotation.y += static_cast<float>(change.x / 8.0f);
        lastMousepositionition = sf::Mouse::getPosition(window);
    }

    const float PLAYER_SPEED = 0.05f;
    float rads = (glm::radians(m_player->rotation.y));
    float rads90 = (glm::radians(m_player->rotation.y + 90));
    if (keyboard.isKeyDown(sf::Keyboard::W)) {
        m_player->position.x -= glm::cos(rads90) * PLAYER_SPEED;
        m_player->position.z -= glm::sin(rads90) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::S)) {
        m_player->position.x += glm::cos(rads90) * PLAYER_SPEED;
        m_player->position.z += glm::sin(rads90) * PLAYER_SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::A)) {
        m_player->position.x -= glm::cos(rads) * PLAYER_SPEED;
        m_player->position.z -= glm::sin(rads) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::D)) {
        m_player->position.x += glm::cos(rads) * PLAYER_SPEED;
        m_player->position.z += glm::sin(rads) * PLAYER_SPEED;
    }
}

void Gameplay::update()
{
    auto packet = makePacket(ServerCommand::PlayerPosition);
    auto id = m_client.getClientId();
    packet.data << id << m_entities[id].position.x << m_entities[id].position.y
                << m_entities[id].position.z;
    m_client.sendPacketToServer(packet);
    handlePackets();
}

void Gameplay::render()
{
    glm::mat4 viewMatrix{1.0f};
    rotateMatrix(&viewMatrix, m_player->rotation);
    translateMatrix(&viewMatrix, -m_player->position);

    glm::mat4 projectionViewMatrix{1.0f};
    projectionViewMatrix = m_projectionMatrix * viewMatrix;
    gl::loadUniform(m_projectionViewLocation, projectionViewMatrix);



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

}

void Gameplay::endGame()
{
    m_cube.destroy();
    m_texture.destroy();
    m_shader.destroy();

    m_client.disconnect();
}

void Gameplay::handlePackets()
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

void Gameplay::handleSnapshot(Packet &packet)
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
            p->active = true;
        }
    }
}

void Gameplay::handlePlayerJoin(Packet &packet)
{
    client_id_t id = 0;
    packet.data >> id;
    m_entities[id].active = true;
    LOGVAR("Client", "Player has joined the game, Client ID:", (int)id);
}

void Gameplay::handlePlayerLeave(Packet &packet)
{
    client_id_t id = 0;
    packet.data >> id;
    m_entities[id].active = false;
    LOGVAR("Client", "Player has left the game, Client ID:", (int)id);
}


/*
        status = m_window.pollEvents(m_keyboard, [this](auto key) {
            if (key == sf::Keyboard::L) {
                m_isMouseLocked = !m_isMouseLocked;
            }
        });
*/