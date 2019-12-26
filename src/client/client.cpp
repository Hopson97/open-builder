#include "client.h"

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

Client::Client()
    : NetworkHost("Client")
{
}

bool Client::init(float aspect)
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
    auto peer = NetworkHost::createAsClient(LOCAL_HOST);
    if (!peer) {
        return false;
    }
    mp_serverPeer = *peer;

    // Set player stuff
    mp_player = &m_entities[NetworkHost::getPeerId()];
    mp_player->position = {CHUNK_SIZE * 2, CHUNK_SIZE * 2 + 1, CHUNK_SIZE * 2};

    // Get world from server
    for (int cy = 0; cy < TEMP_WORLD_HEIGHT; cy++) {
        for (int cz = 0; cz < TEMP_WORLD_WIDTH; cz++) {
            for (int cx = 0; cx < TEMP_WORLD_WIDTH; cx++) {
                ChunkPosition position(cx, cy, cz);
                sendChunkRequest({cx, cy, cz});
            }
        }
    }

    m_projectionMatrix =
        glm::perspective(3.14f / 2.0f, aspect, 0.01f, 10000.0f);
    return true;
}

void Client::handleInput(const sf::Window &window, const Keyboard &keyboard)
{
    static auto lastMousePosition = sf::Mouse::getPosition(window);

    // Handle mouse input
    if (!m_isMouseLocked && window.hasFocus() &&
        sf::Mouse::getPosition(window).y >= 0) {
        auto change = sf::Mouse::getPosition(window) - lastMousePosition;
        mp_player->rotation.x += static_cast<float>(change.y / 8.0f);
        mp_player->rotation.y += static_cast<float>(change.x / 8.0f);
        sf::Mouse::setPosition(
            {(int)window.getSize().x / 2, (int)window.getSize().y / 2}, window);
        lastMousePosition = sf::Mouse::getPosition(window);
    }

    // Handle keyboard input
    float PLAYER_SPEED = 0.05f;
    if (keyboard.isKeyDown(sf::Keyboard::LControl)) {
        PLAYER_SPEED *= 10;
    }

    auto &rotation = mp_player->rotation;
    auto &position = mp_player->position;
    if (keyboard.isKeyDown(sf::Keyboard::W)) {
        position += forwardsVector(rotation) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::S)) {
        position += backwardsVector(rotation) * PLAYER_SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::A)) {
        position += leftVector(rotation) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::D)) {
        position += rightVector(rotation) * PLAYER_SPEED;
    }

    if (keyboard.isKeyDown(sf::Keyboard::Space)) {
        position.y += PLAYER_SPEED * 2;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::LShift)) {
        position.y -= PLAYER_SPEED * 2;
    }

    if (rotation.x < -80.0f) {
        rotation.x = -79.0f;
    }
    else if (rotation.x > 85.0f) {
        rotation.x = 84.0f;
    }
}

void Client::onKeyRelease(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::L) {
        m_isMouseLocked = !m_isMouseLocked;
    }
}

void Client::update()
{
    NetworkHost::tick();
    sendPlayerPosition(mp_player->position);

    for (auto itr = m_chunks.updates.begin(); itr != m_chunks.updates.end();) {
        auto &position = *itr;
        if (findChunkDrawableIndex(position) == -1 &&
            m_chunks.manager.hasNeighbours(position)) {

            m_chunks.bufferables.push_back(
                makeChunkMesh(m_chunks.manager.getChunk(position)));
            itr = m_chunks.updates.erase(itr);
        }
        else {
            itr++;
        }
    }
}

void Client::render()
{
    // Setup matrices
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projectionViewMatrix{1.0f};

    m_basicShader.program.bind();
    rotateMatrix(&viewMatrix, mp_player->rotation);
    translateMatrix(&viewMatrix, -mp_player->position);

    projectionViewMatrix = m_projectionMatrix * viewMatrix;
    gl::loadUniform(m_basicShader.projectionViewLocation, projectionViewMatrix);

    // Render all the entities
    auto drawable = m_cube.getDrawable();
    drawable.bind();
    m_texture.bind();
    for (auto &entity : m_entities) {
        if (entity.active && &entity != mp_player) {
            glm::mat4 modelMatrix{1.0f};
            translateMatrix(&modelMatrix, {entity.position.x, entity.position.y,
                                           entity.position.z});
            gl::loadUniform(m_basicShader.modelLocation, modelMatrix);
            drawable.draw();
        }
    }

    // Render chunks
    m_chunkShader.program.bind();
    m_grassTexture.bind();
    gl::loadUniform(m_chunkShader.projectionViewLocation, projectionViewMatrix);

    // Buffer chunks
    for (auto &chunk : m_chunks.bufferables) {
        m_chunks.drawables.push_back(chunk.createBuffer());
        m_chunks.positions.push_back(chunk.position);
    }
    m_chunks.bufferables.clear();

    // Render them
    for (auto &chunk : m_chunks.drawables) {
        chunk.getDrawable().bindAndDraw();
    }
}

void Client::endGame()
{
    m_cube.destroy();
    m_texture.destroy();
    m_basicShader.program.destroy();
    m_chunkShader.program.destroy();

    for (auto &chunk : m_chunks.drawables) {
        chunk.destroy();
    }
    if (m_status == EngineStatus::Ok) {
        sendDisconnectRequest();
        // Disconnect from the server
    }
}

EngineStatus Client::currentStatus() const
{
    return m_status;
}

int Client::findChunkDrawableIndex(const ChunkPosition &position)
{
    for (int i = 0; i < static_cast<int>(m_chunks.positions.size()); i++) {
        if (m_chunks.positions[i] == position) {
            return i;
        }
    }
    return -1;
}