#include "client.h"

#include "client_config.h"
#include "gl/gl_errors.h"
#include "gl/primitive.h"
#include "gui/widget/label_widget.h"
#include "input/input_state.h"
#include "input/keyboard.h"
#include "world/chunk_mesh_generation.h"
#include <SFML/Window/Window.hpp>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <iomanip>
#include <sstream>

namespace {
    bool isVoxelSelectable(VoxelType voxelType)
    {
        return voxelType == VoxelType::Solid || voxelType == VoxelType::Flora;
    }
} // namespace

Client::Client()
    : NetworkHost("Client")
{
    // clang-format off
    m_commandDispatcher.addCommand(ClientCommand::VoxelUpdate, &Client::onVoxelUpdate);
    m_commandDispatcher.addCommand(ClientCommand::ChunkData, &Client::onChunkData);
    m_commandDispatcher.addCommand(ClientCommand::GameRegistryData, &Client::onGameRegistryData);
    m_commandDispatcher.addCommand(ClientCommand::PlayerJoin, &Client::onPlayerJoin);
    m_commandDispatcher.addCommand(ClientCommand::PlayerLeave, &Client::onPlayerLeave);
    m_commandDispatcher.addCommand(ClientCommand::Snapshot, &Client::onSnapshot);
    m_commandDispatcher.addCommand(ClientCommand::SpawnPoint, &Client::onSpawnPoint);
    m_commandDispatcher.addCommand(ClientCommand::NewPlayerSkin, &Client::onPlayerSkinReceive);
    // clang-format on
}

bool Client::init(const std::string& ipAddress)
{
    // OpenGL stuff
    m_cube = makeCubeVertexArray(1, 2, 1);

    m_selectionBox = makeWireCubeVertexArray(1, 1, 1);

    m_chunkRenderer.init();

    // Selection box shader
    m_selectionShader.program.create("selection", "selection");
    m_selectionShader.program.bind();
    m_selectionShader.modelLocation =
        m_selectionShader.program.getUniformLocation("modelMatrix");
    m_selectionShader.projectionViewLocation =
        m_selectionShader.program.getUniformLocation("projectionViewMatrix");

    // Basic shader
    m_basicShader.program.create("static", "static");
    m_basicShader.program.bind();
    m_basicShader.modelLocation = m_basicShader.program.getUniformLocation("modelMatrix");
    m_basicShader.projectionViewLocation =
        m_basicShader.program.getUniformLocation("projectionViewMatrix");

    // Texture for the player model
    m_errorSkinTexture.create("res/skins/error.png", false);
    m_errorSkinTexture.bind();

    // Set up the server connection
    auto peer = NetworkHost::createAsClient(ipAddress);
    if (!peer) {
        return false;
    }
    mp_serverPeer = *peer;

    // Set player stuff
    mp_player = &m_entities[NetworkHost::getPeerId()];
    mp_player->position = {CHUNK_SIZE * 2, CHUNK_SIZE * 2 + 1, CHUNK_SIZE * 2};

    m_rawPlayerSkin = gl::loadRawImageFile("skins/" + ClientConfig::get().skinName);
    sendPlayerSkin(m_rawPlayerSkin);

    float aspect = static_cast<float>(ClientConfig::get().windowWidth) /
                   static_cast<float>(ClientConfig::get().windowHeight);
    m_projectionMatrix = glm::perspective(3.14f / 2.0f, aspect, 0.01f, 2000.0f);
    return true;
}

void Client::handleInput(const sf::Window& window, const Keyboard& keyboard,
                         const InputState& inputState)
{
    if (!m_hasReceivedGameData) {
        return;
    }
    static auto lastMousePosition = sf::Mouse::getPosition(window);

    if (inputState.isMouseLocked && window.hasFocus() &&
        sf::Mouse::getPosition(window).y >= 0) {
        float verticalSensitivity = ClientConfig::get().verticalSensitivity;
        float horizontalSensitivity = ClientConfig::get().horizontalSensitivity;
        auto change = sf::Mouse::getPosition(window) - lastMousePosition;

        mp_player->rotation.x +=
            static_cast<float>(change.y / 8.0f * verticalSensitivity);
        mp_player->rotation.y +=
            static_cast<float>(change.x / 8.0f * horizontalSensitivity);
        sf::Mouse::setPosition({(int)window.getSize().x / 2, (int)window.getSize().y / 2},
                               window);

// This fixes mouse jittering on mac
#ifndef __APPLE__
        lastMousePosition = sf::Mouse::getPosition(window);
#else
        lastMousePosition.x = (int)window.getSize().x / 2;
        lastMousePosition.y = (int)window.getSize().y / 2;
#endif
    }

    // Handle keyboard input
    float PLAYER_SPEED = 5.0f;
    if (keyboard.isKeyDown(sf::Keyboard::LControl)) {
        PLAYER_SPEED *= 10;
    }

    // Handle mouse input
    auto& rotation = mp_player->rotation;
    auto& velocity = mp_player->velocity;
    if (keyboard.isKeyDown(sf::Keyboard::W)) {
        velocity += forwardsVector(rotation) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::S)) {
        velocity += backwardsVector(rotation) * PLAYER_SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::A)) {
        velocity += leftVector(rotation) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::D)) {
        velocity += rightVector(rotation) * PLAYER_SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::Space)) {
        velocity.y += PLAYER_SPEED * 2;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::LShift)) {
        velocity.y -= PLAYER_SPEED * 2;
    }
    if (rotation.x < -80.0f) {
        rotation.x = -79.9f;
    }
    else if (rotation.x > 85.0f) {
        rotation.x = 84.9f;
    }
}

void Client::onMouseRelease(sf::Mouse::Button button)
{
    // Handle voxel removal/ voxel placing events

    auto voxels =
        getIntersectedVoxels(mp_player->position, forwardsVector(mp_player->rotation), 8);

    VoxelPosition& previous = voxels.at(0);
    for (auto& position : voxels) {
        auto& voxel = m_voxelData.getVoxelData(m_chunks.manager.getVoxel(position));

        if (isVoxelSelectable(voxel.type)) {
            VoxelUpdate voxelUpdate;
            voxelUpdate.voxel = button == sf::Mouse::Left ? 0 : 1;
            if (button == sf::Mouse::Left) {
                voxelUpdate.position = position;
            }
            else if (previous == toVoxelPosition(mp_player->position)) {
                // prevents players from replacing voxels they're inside of
                break;
            }
            else {
                voxelUpdate.position = previous;
            }
            voxelUpdate.position = button == sf::Mouse::Left ? position : previous;
            m_chunks.voxelUpdates.push_back(voxelUpdate);
            sendVoxelUpdate(voxelUpdate);
            break;
        }
        previous = position;
    }
}

void Client::update(float dt)
{
    NetworkHost::tick();
    if (!m_hasReceivedGameData) {
        return;
    }

    mp_player->position += mp_player->velocity * dt;
    mp_player->velocity *= 0.99 * dt;

    sendPlayerPosition(mp_player->position);

    // Update voxels
    for (auto& voxelUpdate : m_chunks.voxelUpdates) {
        auto chunkPosition = toChunkPosition(voxelUpdate.position);
        m_chunks.manager.ensureNeighbours(chunkPosition);
        m_chunks.manager.setVoxel(voxelUpdate.position, voxelUpdate.voxel);
        m_chunks.updates.push_back(chunkPosition);

        auto p = chunkPosition;
        auto localVoxelPostion = toLocalVoxelPosition(voxelUpdate.position);
        if (localVoxelPostion.x == 0) {
            m_chunks.updates.push_back({p.x - 1, p.y, p.z});
        }
        else if (localVoxelPostion.x == CHUNK_SIZE - 1) {
            m_chunks.updates.push_back({p.x + 1, p.y, p.z});
        }

        if (localVoxelPostion.y == 0) {
            m_chunks.updates.push_back({p.x, p.y - 1, p.z});
        }
        else if (localVoxelPostion.y == CHUNK_SIZE - 1) {
            m_chunks.updates.push_back({p.x, p.y + 1, p.z});
        }

        if (localVoxelPostion.z == 0) {
            m_chunks.updates.push_back({p.x, p.y, p.z - 1});
        }
        else if (localVoxelPostion.z == CHUNK_SIZE - 1) {
            m_chunks.updates.push_back({p.x, p.y, p.z + 1});
        }
    }
    m_chunks.voxelUpdates.clear();

    auto playerChunk = worldToChunkPosition(mp_player->position);
    auto distanceToPlayer = [&playerChunk](const ChunkPosition& chunkPosition) {
        return glm::abs(playerChunk.x - chunkPosition.x) +
               glm::abs(playerChunk.y - chunkPosition.y) +
               glm::abs(playerChunk.z - chunkPosition.z);
    };

    if (!m_chunks.updates.empty()) {
        // Sort chunk updates by distance if the update vector is not
        // sorted already
        if (!std::is_sorted(m_chunks.updates.begin(), m_chunks.updates.end(),
                            [&](const auto& a, const auto& b) {
                                return distanceToPlayer(a) < distanceToPlayer(b);
                            })) {
            // Remove non-unique elements
            std::unordered_set<ChunkPosition, ChunkPositionHash> updates;
            for (auto& update : m_chunks.updates) {
                updates.insert(update);
            }

            m_chunks.updates.assign(updates.cbegin(), updates.cend());

            // Sort it to find chunk mesh cloest to the player
            std::sort(m_chunks.updates.begin(), m_chunks.updates.end(),
                      [&](const auto& a, const auto& b) {
                          return distanceToPlayer(a) < distanceToPlayer(b);
                      });
        }

        if (m_noMeshingCount != m_chunks.updates.size()) {
            m_voxelMeshing = false;
        }

        // Find first "meshable" chunk
        int count = 0;
        if (!m_voxelMeshing) {
            m_noMeshingCount = 0;
            for (auto itr = m_chunks.updates.cbegin(); itr != m_chunks.updates.cend();) {
                if (m_chunks.manager.hasNeighbours(*itr)) {
                    auto& chunk = m_chunks.manager.getChunk(*itr);

                    auto buffer = makeChunkMesh(chunk, m_voxelData);
                    m_chunkRenderer.updateMesh(*itr, std::move(buffer));
                    itr = m_chunks.updates.erase(itr);

                    // Break so that the game still runs while world is
                    // being built
                    // @TODO: Work out a way to make this concurrent (aka
                    // run seperate from rest of application)
                    if (count++ > 3) {
                        break;
                    }
                }
                else {
                    m_noMeshingCount++;
                    itr++;
                }
            }
            if (m_noMeshingCount == m_chunks.updates.size()) {
                m_voxelMeshing = true;
            }
        }
    }

    // Determine if a player is selecting a voxel & if so, which
    m_voxelSelected = false;
    auto voxels =
        getIntersectedVoxels(mp_player->position, forwardsVector(mp_player->rotation), 8);
    for (auto& position : voxels) {
        auto& voxel = m_voxelData.getVoxelData(m_chunks.manager.getVoxel(position));
        if (isVoxelSelectable(voxel.type)) {
            m_currentSelectedVoxelPos = position;
            m_voxelSelected = true;
            break;
        }
    }
}

void Client::render()
{
    // @TODO [Hopson] Clean this up
    if (!m_hasReceivedGameData) {
        return;
    }
    // Setup matrices
    m_basicShader.program.bind();
    glm::mat4 playerProjectionView = createProjectionViewMatrix(
        mp_player->position, mp_player->rotation, m_projectionMatrix);

    gl::loadUniform(m_basicShader.projectionViewLocation, playerProjectionView);

    // Update the viewing frustum for frustum culling
    m_frustum.update(playerProjectionView);

    // Render all the entities
    auto drawable = m_cube.getDrawable();
    drawable.bind();

    for (auto& ent : m_entities) {

        if (ent.active && &ent != mp_player) {
            if (ent.playerSkin.textureExists()) {
                ent.playerSkin.bind();
            }
            else {
                m_errorSkinTexture.bind();
            }

            glm::mat4 modelMatrix{1.0f};
            translateMatrix(modelMatrix,
                            {ent.position.x, ent.position.y, ent.position.z});
            gl::loadUniform(m_basicShader.modelLocation, modelMatrix);
            drawable.draw();
        }
    };
    // Render chunks
    m_voxelTextures.bind();

    bool isPlayerInWater =
        m_chunks.manager.getVoxel(toVoxelPosition(mp_player->position)) ==
        m_voxelData.getVoxelId(CommonVoxel::Water);
    auto result = m_chunkRenderer.renderChunks(mp_player->position, m_frustum,
                                               playerProjectionView, isPlayerInWater);

    // Render selection box
    if (m_voxelSelected) {
        glCheck(glEnable(GL_BLEND));
        glCheck(glEnable(GL_LINE_SMOOTH));
        glCheck(glLineWidth(2.0));
        m_selectionShader.program.bind();
        glm::mat4 modelMatrix{1.0};
        float size = 1.005f;
        translateMatrix(modelMatrix, {m_currentSelectedVoxelPos.x - (size - 1) / 2,
                                      m_currentSelectedVoxelPos.y - (size - 1) / 2,
                                      m_currentSelectedVoxelPos.z - (size - 1) / 2});
        scaleMatrix(modelMatrix, size);
        gl::loadUniform(m_selectionShader.modelLocation, modelMatrix);
        gl::loadUniform(m_selectionShader.projectionViewLocation, playerProjectionView);
        m_selectionBox.getDrawable().bindAndDraw(GL_LINES);
        glCheck(glDisable(GL_BLEND));
    }
}

void Client::endGame()
{
    if (mp_serverPeer) {
        NetworkHost::disconnectFromPeer(mp_serverPeer);
    }
}
