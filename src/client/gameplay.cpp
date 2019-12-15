#include "gameplay.h"

#include "input/keyboard.h"
#include "world/chunk_mesh_generation.h"
#include <SFML/Window/Mouse.hpp>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <thread>

namespace {
gl::VertexArray createCube(float height = 1)
{
    std::vector<GLfloat> vertices = {
        // Front
        1, height, 1, 0, height, 1, 0, 0, 1, 1, 0, 1,
        // Left
        0, height, 1, 0, height, 0, 0, 0, 0, 0, 0, 1,
        // Back
        0, height, 0, 1, height, 0, 1, 0, 0, 0, 0, 0,
        // Right
        1, height, 0, 1, height, 1, 1, 0, 1, 1, 0, 0,
        // Top
        1, height, 0, 0, height, 0, 0, height, 1, 1, height, 1,
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

Gameplay::Gameplay()
    : NetworkHost("Client")
{
}

bool Gameplay::init(float aspect)
{
    // OpenGL stuff
    m_cube = createCube(2);

    // Basic shader
    m_basicShader.program.create("static", "static");
    m_basicShader.program.bind();
    m_basicShader.modelLocation =
        m_basicShader.program.getUniformLocation("modelMatrix");
    m_basicShader.projectionViewLocation =
        m_basicShader.program.getUniformLocation("projectionViewMatrix");

    // Chunk shader
    m_chunkShader.program.create("chunk", "chunk");
    m_chunkShader.program.bind();
    m_chunkShader.projectionViewLocation =
        m_chunkShader.program.getUniformLocation("projectionViewMatrix");


    // Texture for the player model
    m_texture.create("player");
    m_texture.bind();

    // Texture for grass
    m_grassTexture.create("grass");
    m_grassTexture.bind();

    // Set up the server connection
    auto peer = NetworkHost::connectToServer(LOCAL_HOST);
    if (!peer) {
        return false;
    }
    mp_serverPeer = *peer;
    mp_player = &m_entities[NetworkHost::getPeerId()];
    mp_player->position = {0, 10, 0};

    std::vector<Chunk *> chunks;

    // Set up world
    for (int cy = 0; cy < 5; cy++) {
        for (int cz = 0; cz < 5; cz++) {
            for (int cx = 0; cx < 5; cx++) {
                auto& chunk = m_chunks.addChunk({cx, cy, cz});
                chunks.push_back(&chunk);
                for (int y = 0; y < CHUNK_SIZE; y++) {
                    for (int z = 0; z < CHUNK_SIZE; z++) {
                        for (int x = 0; x < CHUNK_SIZE; x++) {
                            chunk.qSetBlock({x, y, z}, 1);
                        }
                    }
                }
            }
        }
    }

    for (auto &chunk : chunks) {
        m_chunkRenders.push_back(makeChunkMesh(*chunk));
    }

    m_projectionMatrix = glm::perspective(3.14f / 2.0f, aspect, 0.01f, 10000.0f);
    return true;
}

void Gameplay::handleInput(const sf::Window &window, const Keyboard &keyboard)
{
    static auto lastMousepositionition = sf::Mouse::getPosition(window);

    // Handle mouse input
    if (!m_isMouseLocked && window.hasFocus()) {
        auto change = sf::Mouse::getPosition(window) - lastMousepositionition;
        mp_player->rotation.x += static_cast<float>(change.y / 8.0f);
        mp_player->rotation.y += static_cast<float>(change.x / 8.0f);
        sf::Mouse::setPosition(
            {(int)window.getSize().x / 2, (int)window.getSize().y / 2}, window);
        lastMousepositionition = sf::Mouse::getPosition(window);
    }

    // Handle keyboard input
    const float PLAYER_SPEED = 0.05f;
    float rads = (glm::radians(mp_player->rotation.y));
    float rads90 = (glm::radians(mp_player->rotation.y + 90));
    if (keyboard.isKeyDown(sf::Keyboard::W)) {
        mp_player->position.x -= glm::cos(rads90) * PLAYER_SPEED;
        mp_player->position.z -= glm::sin(rads90) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::S)) {
        mp_player->position.x += glm::cos(rads90) * PLAYER_SPEED;
        mp_player->position.z += glm::sin(rads90) * PLAYER_SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::A)) {
        mp_player->position.x -= glm::cos(rads) * PLAYER_SPEED;
        mp_player->position.z -= glm::sin(rads) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::D)) {
        mp_player->position.x += glm::cos(rads) * PLAYER_SPEED;
        mp_player->position.z += glm::sin(rads) * PLAYER_SPEED;
    }

    if (keyboard.isKeyDown(sf::Keyboard::Q)) {
        mp_player->position.y += 0.1f;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::E)) {
        mp_player->position.y -= 0.1f;
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
    sf::Packet packet;
    packet << ServerCommand::PlayerPosition << NetworkHost::getPeerId()
           << mp_player->position.x << mp_player->position.y
           << mp_player->position.z;
    NetworkHost::sendToPeer(*mp_serverPeer, packet, 0, 0);

    NetworkHost::tick();
}

void Gameplay::render()
{
    // Setup matrices
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projectionViewMatrix{1.0f};

    m_basicShader.program.bind();
    rotateMatrix(&viewMatrix, mp_player->rotation);
    translateMatrix(&viewMatrix, -mp_player->position);

    projectionViewMatrix = m_projectionMatrix * viewMatrix;
    gl::loadUniform(m_basicShader.projectionViewLocation, projectionViewMatrix);

    // Render all the players
    auto drawable = m_cube.getDrawable();
    drawable.bind();
    m_texture.bind();
    for (auto &p : m_entities) {
        if (p.active && &p != mp_player) {
            glm::mat4 modelMatrix{1.0f};
            translateMatrix(&modelMatrix,
                            {p.position.x, p.position.y, p.position.z});
            gl::loadUniform(m_basicShader.modelLocation, modelMatrix);
            drawable.draw();
        }
    }

    // Render the world
    m_chunkShader.program.bind();
    m_grassTexture.bind();
    gl::loadUniform(m_chunkShader.projectionViewLocation, projectionViewMatrix);
    for (auto &chunk : m_chunkRenders) {
        chunk.getDrawable().bindAndDraw();
    }
}

void Gameplay::endGame()
{
    m_cube.destroy();
    m_texture.destroy();
    m_basicShader.program.destroy();
    m_chunkShader.program.destroy();
    for (auto &chunk : m_chunkRenders) {
        chunk.destroy();
    }

    if (m_status == EngineStatus::Ok) {
        // Disconnect from the server
        sf::Packet packet;
        packet << ServerCommand::Disconnect << NetworkHost::getPeerId();
        if (!sendToPeer(*mp_serverPeer, packet, 0, ENET_PACKET_FLAG_RELIABLE)) {
            LOG("Client", "Failed to send disconnect packet");
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(64));

        NetworkHost::disconnectFromPeer(*mp_serverPeer);
    }
    NetworkHost::destroy();
}

EngineStatus Gameplay::currentStatus() const
{
    return m_status;
}

//
//      Network Related Code
//
void Gameplay::onPeerConnect([[maybe_unused]] ENetPeer &peer)
{
}

void Gameplay::onPeerDisconnect([[maybe_unused]] ENetPeer &peer)
{
    m_status = EngineStatus::ExitServerDisconnect;
}

void Gameplay::onPeerTimeout([[maybe_unused]] ENetPeer &peer)
{
    m_status = EngineStatus::ExitServerTimeout;
}

void Gameplay::onCommandRecieve(sf::Packet &packet, command_t command)
{
    switch (static_cast<ClientCommand>(command)) {
        case ClientCommand::PlayerJoin:
            onPlayerJoin(packet);
            break;

        case ClientCommand::PlayerLeave:
            onPlayerLeave(packet);
            break;

        case ClientCommand::Snapshot:
            onSnapshot(packet);
            break;

        case ClientCommand::PeerId:
            break;
    }
}

void Gameplay::onPlayerJoin(sf::Packet &packet)
{
    peer_id_t id;
    packet >> id;
    m_entities[id].active = true;

    LOGVAR("Client", "Player joined, client id: ", (int)id);
}

void Gameplay::onPlayerLeave(sf::Packet &packet)
{
    peer_id_t id;
    packet >> id;
    m_entities[id].active = false;

    LOGVAR("Client", "Player left, client id: ", (int)id);
}

void Gameplay::onSnapshot(sf::Packet &packet)
{
    u16 updateEntityCount = 0;
    packet >> updateEntityCount;
    for (u16 i = 0; i < updateEntityCount; i++) {
        peer_id_t id = 0;
        float x, y, z;
        packet >> id >> x >> y >> z;
        if (id != NetworkHost::getPeerId()) {
            auto *p = &m_entities[id];
            p->position = {x, y, z};
            p->active = true;
        }
    }
}
