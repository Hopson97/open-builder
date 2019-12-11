#include "gameplay.h"

#include "input/keyboard.h"
#include <SFML/Window/Mouse.hpp>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <common/network/packet.h>

namespace {
gl::VertexArray createCube()
{
    std::vector<GLfloat> vertices = {
        // Front
        1, 2, 1, 0, 2, 1, 0, 0, 1, 1, 0, 1,
        // Left
        0, 2, 1, 0, 2, 0, 0, 0, 0, 0, 0, 1,
        // Back
        0, 2, 0, 1, 2, 0, 1, 0, 0, 0, 0, 0,
        // Right
        1, 2, 0, 1, 2, 1, 1, 0, 1, 1, 0, 0,
        // Top
        1, 2, 0, 0, 2, 0, 0, 2, 1, 1, 2, 1,
        // Bottom
        0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1,
        // this just stops clang format from making the array long
    };
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
                             {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f});
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

bool Gameplay::init(float aspect)
{
    m_cube = createCube();

    m_shader.create("static", "static");
    m_shader.bind();
    m_modelLocation = m_shader.getUniformLocation("modelMatrix");
    m_projectionViewLocation =
        m_shader.getUniformLocation("projectionViewMatrix");

    m_texture.create("player");
    m_texture.bind();

    // Connect to the server
    m_host = enet_host_create(nullptr, 1, 2, 0, 0);
    ENetAddress address{0};
    enet_address_set_host(&address, "127.0.0.1");
    address.port = DEFAULT_PORT;
    m_serverPeer = enet_host_connect(m_host, &address, 2, 0);
    LOG("Client", "Connection request sent to server.");

    ENetEvent event;
    if (enet_host_service(m_host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT) {
        LOG("Client", "Connection was a success\n");
    }

    /*
    // Ensure connection is recieved and player pointer is created
    sf::Clock timeout;
    while (timeout.getElapsedTime().asSeconds() < 5.0f) {
        ENetEvent event;
        if (enet_host_service(m_host, &event, 100)) {
            if (event.type == ENET_EVENT_TYPE_RECEIVE) {
                ENetPacket *packet = event.packet;
                sf::Packet pkt;
                pkt.append(packet->data, packet->dataLength);
                ClientCommand command;
                pkt >> command;
                if (command == ClientCommand::AcceptConnection) {
                    client_id_t id;
                    pkt >> id;
                    m_player = &m_entities[id];
                    m_clientId = id;
                    LOGVAR("Client",
                           "Connection to server accepted. Client ID: ",
                           (int)m_clientId);
                    break;
                }
            }
        }
    }
    */
    if (!m_player) {
        m_player = &m_entities[0];
    //    return false;
    }
    m_projectionMatrix = glm::perspective(3.14f / 2.0f, aspect, 0.01f, 100.0f);
    return true;
}

void Gameplay::handleInput(const sf::Window &window, const Keyboard &keyboard)
{
    static auto lastMousepositionition = sf::Mouse::getPosition(window);

    // Handle mouse input
    if (!m_isMouseLocked && window.hasFocus()) {
        auto change = sf::Mouse::getPosition(window) - lastMousepositionition;
        m_player->rotation.x += static_cast<float>(change.y / 8.0f);
        m_player->rotation.y += static_cast<float>(change.x / 8.0f);
        sf::Mouse::setPosition(
            {(int)window.getSize().x / 2, (int)window.getSize().y / 2}, window);
        lastMousepositionition = sf::Mouse::getPosition(window);
    }

    // Handle keyboard input
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

    if (keyboard.isKeyDown(sf::Keyboard::Q)) {
        m_player->position.y += 0.1f;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::E)) {
        m_player->position.y -= 0.1f;
    }
}

void Gameplay::onKeyRelease(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::L) {
        m_isMouseLocked = !m_isMouseLocked;
    }
}

void Gameplay::update()
{
    // Send the position of this player to the server

    /*
    auto packet = makePacket(ServerCommand::PlayerPosition);
    auto id = m_client.getClientId();
    auto &entity = m_entities[id];
    packet.data << id << m_entities[id].position.x << m_entities[id].position.y
                << entity.position.z;
    m_client.sendPacketToServer(packet);
    */

    // Recieve updates from the server
    handlePackets();
}

void Gameplay::render()
{
    // Setup matrices
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projectionViewMatrix{1.0f};

    rotateMatrix(&viewMatrix, m_player->rotation);
    translateMatrix(&viewMatrix, -m_player->position);

    projectionViewMatrix = m_projectionMatrix * viewMatrix;
    gl::loadUniform(m_projectionViewLocation, projectionViewMatrix);

    // Render all the players
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

    // Render the """"""""world"""""""""""
    glm::mat4 modelMatrix{1.0f};
    gl::loadUniform(m_modelLocation, modelMatrix);
    drawable.draw();
}

void Gameplay::endGame()
{
    m_cube.destroy();
    m_texture.destroy();
    m_shader.destroy();

    LOG("Client", "Disconnecting from server.");

    sf::Packet packet;
    packet << ServerCommand::Disconnect << m_clientId;
    ENetPacket *enetpacket = enet_packet_create(
        packet.getData(), packet.getDataSize(),
        ENET_PACKET_FLAG_RELIABLE | ENET_PACKET_FLAG_NO_ALLOCATE);
    enet_peer_send(m_serverPeer, 0, enetpacket);
    enet_host_flush(m_host);
    enet_packet_destroy(enetpacket);

    // Server diconnect
    enet_peer_disconnect(m_serverPeer, 0);
    ENetEvent event;
    while (enet_host_service(m_host, &event, 1000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_RECEIVE:
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                LOG("Client", "Disconnect was a success")
                break;
            case ENET_EVENT_TYPE_DISCONNECT_TIMEOUT:
                break;
            default:
                break;
        }
    }
    enet_peer_reset(m_serverPeer);
    enet_host_destroy(m_host);
}
