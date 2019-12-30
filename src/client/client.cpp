#include "client.h"

#include "gl/primitive.h"
#include "input/keyboard.h"
#include "world/chunk_mesh_generation.h"
#include <SFML/Window/Mouse.hpp>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <thread>

Client::Client()
    : NetworkHost("Client")
{
}

bool Client::init(float aspect)
{
    // OpenGL stuff
    m_cube = makeCubeVertexArray(1, 2, 1);

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
    float PLAYER_SPEED = 0.2f;
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
        // std::cout << position << std::endl;
    }

    /*
        if (rotation.x < -80.0f) {
            rotation.x = -79.0f;
        }
        else if (rotation.x > 85.0f) {
            rotation.x = 84.0f;
        }
    */
}

void Client::onKeyRelease(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::L) {
        m_isMouseLocked = !m_isMouseLocked;
    }
}

void Client::onMouseRelease(sf::Mouse::Button button, [[maybe_unused]] int x,
                            [[maybe_unused]] int y)
{
    // Handle block removal/ block placing events
    Ray ray(mp_player->position, mp_player->rotation);
    for (; ray.getLength() < 8; ray.step()) {
        auto blockPosition = toBlockPosition(ray.getEndpoint());
        if (m_chunks.manager.getBlock(blockPosition) == 1) {

            BlockUpdate blockUpdate;
            blockUpdate.block = button == sf::Mouse::Left ? 0 : 1;
            blockUpdate.position = button == sf::Mouse::Left
                                       ? blockPosition
                                       : toBlockPosition(ray.getLastPoint());
            m_chunks.blockUpdates.push_back(blockUpdate);
            break;
        }
    }
}

void Client::update()
{
    NetworkHost::tick();
    sendPlayerPosition(mp_player->position);
    // Update blocks
    for (auto &blockUpdate : m_chunks.blockUpdates) {
        auto chunkPosition = toChunkPosition(blockUpdate.position);
        m_chunks.manager.ensureNeighbours(chunkPosition);
        m_chunks.manager.setBlock(blockUpdate.position, blockUpdate.block);
        m_chunks.updates.push_back(chunkPosition);
    }
    m_chunks.blockUpdates.clear();

    auto playerChunk = worldToChunkPosition(mp_player->position);
    auto distanceToPlayer = [&playerChunk](const ChunkPosition &chunkPosition) {
        return glm::abs(playerChunk.x - chunkPosition.x) +
               glm::abs(playerChunk.y - chunkPosition.y) +
               glm::abs(playerChunk.z - chunkPosition.z);
    };

    if (!m_chunks.updates.empty()) {
        // Sort chunk updates by distance if the update vector is not sorted
        // already
        if (!std::is_sorted(m_chunks.updates.begin(), m_chunks.updates.end(),
                            [&](const auto &a, const auto &b) {
                                return distanceToPlayer(a) <
                                       distanceToPlayer(b);
                            })) {
            // Remove unique elements
            std::unordered_set<ChunkPosition, ChunkPositionHash> updates;
            for (auto &update : m_chunks.updates) {
                updates.insert(update);
            }

            m_chunks.updates.assign(updates.cbegin(), updates.cend());

            // Sort it to find chunk mesh cloest to the player
            std::sort(m_chunks.updates.begin(), m_chunks.updates.end(),
                      [&](const auto &a, const auto &b) {
                          return distanceToPlayer(a) < distanceToPlayer(b);
                      });
        }

        // Find first "meshable" chunk
        for (auto itr = m_chunks.updates.begin();
             itr != m_chunks.updates.end();) {
            if (m_chunks.manager.hasNeighbours(*itr) &&
                m_frustum.chunkIsInFrustum(*itr)) {
                auto &chunk = m_chunks.manager.getChunk(*itr);
                auto buffer = makeChunkMesh(chunk);
                m_chunks.bufferables.push_back(buffer);
                deleteChunkRenderable(*itr);
                m_chunks.updates.erase(itr);

                // Break so that the game still runs while world is being built
                // TODO: Work out a way to make this concurrent (aka run
                // seperate from rest of application)
                break;
            }
            else {
                itr++;
            }
        }
    }
}

void Client::render()
{
    // Setup matrices
    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projectionViewMatrix{1.0f};

    m_basicShader.program.bind();
    rotateMatrix(viewMatrix, mp_player->rotation);
    translateMatrix(viewMatrix, -mp_player->position);

    projectionViewMatrix = m_projectionMatrix * viewMatrix;
    gl::loadUniform(m_basicShader.projectionViewLocation, projectionViewMatrix);

    m_frustum.update(projectionViewMatrix);

    // Render all the entities
    auto drawable = m_cube.getDrawable();
    drawable.bind();
    m_texture.bind();
    for (auto &ent : m_entities) {
        if (ent.active && &ent != mp_player) {
            glm::mat4 modelMatrix{1.0f};
            translateMatrix(modelMatrix,
                            {ent.position.x, ent.position.y, ent.position.z});
            gl::loadUniform(m_basicShader.modelLocation, modelMatrix);
            drawable.draw();
        }
    }

    // Render chunks
    m_chunkShader.program.bind();
    m_grassTexture.bind();
    gl::loadUniform(m_chunkShader.projectionViewLocation, projectionViewMatrix);

    // Buffer chunks
    for (auto &chunkMesh : m_chunks.bufferables) {
        if (chunkMesh.indicesCount > 0) {
            m_chunks.drawables.push_back(chunkMesh.createBuffer());
            m_chunks.positions.push_back(chunkMesh.position);
        }
    }
    m_chunks.bufferables.clear();

    // Render them (if in view)
    assert(m_chunks.drawables.size() == m_chunks.positions.size());
    for (unsigned i = 0; i < m_chunks.drawables.size(); i++) {
        if (m_frustum.chunkIsInFrustum(m_chunks.positions[i])) {
            m_chunks.drawables[i].getDrawable().bindAndDraw();
        }
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
    NetworkHost::disconnectFromPeer(mp_serverPeer);
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

void Client::deleteChunkRenderable(const ChunkPosition &position)
{
    auto index = findChunkDrawableIndex(position);
    if (index > -1) {
        m_chunks.drawables[index].destroy();

        // As the chunk renders need not be a sorted array, "swap and pop"
        // can be used
        // More efficent (and maybe safer) than normal deletion
        std::iter_swap(m_chunks.drawables.begin() + index,
                       m_chunks.drawables.end() - 1);
        std::iter_swap(m_chunks.positions.begin() + index,
                       m_chunks.positions.end() - 1);
        m_chunks.drawables.pop_back();
        m_chunks.positions.pop_back();
    }
}