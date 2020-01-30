#include "client.h"

#include "gl/gl_errors.h"
#include "gl/primitive.h"
#include "input/keyboard.h"
#include "world/chunk_mesh_generation.h"
#include <SFML/Window/Mouse.hpp>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <thread>

#include "client_config.h"

namespace {
int findChunkDrawableIndex(const ChunkPosition &position,
                           const std::vector<ChunkDrawable> &drawables)
{
    for (int i = 0; i < static_cast<int>(drawables.size()); i++) {
        if (drawables[i].position == position) {
            return i;
        }
    }
    return -1;
}

void deleteChunkRenderable(const ChunkPosition &position,
                           std::vector<ChunkDrawable> &drawables)
{
    auto index = findChunkDrawableIndex(position, drawables);
    if (index > -1) {
        drawables[index].vao.destroy();

        // As the chunk renders need not be a sorted array, "swap and pop"
        // can be used
        // More efficent (and maybe safer) than normal deletion
        std::iter_swap(drawables.begin() + index, drawables.end() - 1);
        drawables.pop_back();
    }
}

void renderChunks(const std::vector<ChunkDrawable> &chunks,
                  const ViewFrustum &frustum)
{
    for (const auto &chunk : chunks) {
        if (frustum.chunkIsInFrustum(chunk.position)) {
            chunk.vao.getDrawable().bindAndDraw();
        }
    }
}

bool isVoxelSelectable(VoxelType voxelType)
{
    return voxelType == VoxelType::Solid || voxelType == VoxelType::Flora;
}
} // namespace

Client::Client()
    : NetworkHost("Client")
{
    // clang-format off
    m_commandDispatcher.addCommand(ClientCommand::BlockUpdate, &Client::onBlockUpdate);
    m_commandDispatcher.addCommand(ClientCommand::ChunkData, &Client::onChunkData);
    m_commandDispatcher.addCommand(ClientCommand::GameRegistryData, &Client::onGameRegistryData);
    m_commandDispatcher.addCommand(ClientCommand::PlayerJoin, &Client::onPlayerJoin);
    m_commandDispatcher.addCommand(ClientCommand::PlayerLeave, &Client::onPlayerLeave);
    m_commandDispatcher.addCommand(ClientCommand::Snapshot, &Client::onSnapshot);
    m_commandDispatcher.addCommand(ClientCommand::SpawnPoint, &Client::onSpawnPoint);
    m_commandDispatcher.addCommand(ClientCommand::NewPlayerSkin, &Client::onPlayerSkinReceive);
    // clang-format on

    auto luaGuiAPI = m_lua.addTable("GUI");
    luaGuiAPI["addImage"] = [&](sol::userdata img) { m_gui.addImage(img); };

    m_gui.addUsertypes(luaGuiAPI);

    m_lua.lua["update"] = 3;
    m_lua.runLuaFile("game/client/main.lua");
}

bool Client::init(const ClientConfig &config, float aspect)
{
    // OpenGL stuff
    m_cube = makeCubeVertexArray(1, 2, 1);

    m_selectionBox = makeWireCubeVertexArray(1, 1, 1);

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
    m_basicShader.modelLocation =
        m_basicShader.program.getUniformLocation("modelMatrix");
    m_basicShader.projectionViewLocation =
        m_basicShader.program.getUniformLocation("projectionViewMatrix");

    // Chunk shader
    m_chunkShader.program.create("chunk", "chunk");
    m_chunkShader.program.bind();
    m_chunkShader.projectionViewLocation =
        m_chunkShader.program.getUniformLocation("projectionViewMatrix");

    // Chunk shader
    m_fluidShader.program.create("water", "chunk");
    m_fluidShader.program.bind();
    m_fluidShader.projectionViewLocation =
        m_fluidShader.program.getUniformLocation("projectionViewMatrix");
    m_fluidShader.timeLocation =
        m_fluidShader.program.getUniformLocation("time");

    // Texture for the player model
    m_errorSkinTexture.create("skins/error");
    m_errorSkinTexture.bind();

    m_texturePack = config.texturePack;

    // Set up the server connection
    auto peer = NetworkHost::createAsClient(config.serverIp);
    if (!peer) {
        return false;
    }
    mp_serverPeer = *peer;

    // Set player stuff
    mp_player = &m_entities[NetworkHost::getPeerId()];
    mp_player->position = {CHUNK_SIZE * 2, CHUNK_SIZE * 2 + 1, CHUNK_SIZE * 2};

    m_rawPlayerSkin = gl::loadRawImageFile("skins/" + config.skinName);
    sendPlayerSkin(m_rawPlayerSkin);

    m_projectionMatrix = glm::perspective(3.14f / 2.0f, aspect, 0.01f, 2000.0f);
    return true;
}

void Client::handleInput(const sf::Window &window, const Keyboard &keyboard)
{
    if (!m_hasReceivedGameData) {
        return;
    }
    static auto lastMousePosition = sf::Mouse::getPosition(window);

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
    float PLAYER_SPEED = 5.0f;
    if (keyboard.isKeyDown(sf::Keyboard::LControl)) {
        PLAYER_SPEED *= 10;
    }

    // Handle mouse input
    auto &rotation = mp_player->rotation;
    auto &velocity = mp_player->velocity;
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
        std::cout << mp_player->position << std::endl;
    }
    if (rotation.x < -80.0f) {
        rotation.x = -79.9f;
    }
    else if (rotation.x > 85.0f) {
        rotation.x = 84.9f;
    }
}

void Client::onMouseRelease(sf::Mouse::Button button, [[maybe_unused]] int x,
                            [[maybe_unused]] int y)
{
    // Handle block removal/ block placing events

    // Create a "ray"
    Ray ray(mp_player->position, mp_player->rotation);

    // Step the ray until it hits a block/ reaches maximum length
    for (; ray.getLength() < 8; ray.step()) {
        auto rayBlockPosition = toBlockPosition(ray.getEndpoint());
        auto &voxel = m_voxelData.getVoxelData(
            m_chunks.manager.getBlock(rayBlockPosition));
        if (isVoxelSelectable(voxel.type)) {
            BlockUpdate blockUpdate;
            blockUpdate.block = button == sf::Mouse::Left ? 0 : 1;
            blockUpdate.position = button == sf::Mouse::Left
                                       ? rayBlockPosition
                                       : toBlockPosition(ray.getLastPoint());
            m_chunks.blockUpdates.push_back(blockUpdate);
            sendBlockUpdate(blockUpdate);
            break;
        }
    }
}

void Client::onKeyRelease(sf::Keyboard::Key key)
{
    switch (key) {
        case sf::Keyboard::L:
            m_isMouseLocked = !m_isMouseLocked;
            break;

        case sf::Keyboard::P:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;

        case sf::Keyboard::F:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;

        default:
            break;
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

    // Update blocks
    for (auto &blockUpdate : m_chunks.blockUpdates) {
        auto chunkPosition = toChunkPosition(blockUpdate.position);
        m_chunks.manager.ensureNeighbours(chunkPosition);
        m_chunks.manager.setBlock(blockUpdate.position, blockUpdate.block);
        m_chunks.updates.push_back(chunkPosition);

        auto p = chunkPosition;
        auto localBlockPostion = toLocalBlockPosition(blockUpdate.position);
        if (localBlockPostion.x == 0) {
            m_chunks.updates.push_back({p.x - 1, p.y, p.z});
        }
        else if (localBlockPostion.x == CHUNK_SIZE - 1) {
            m_chunks.updates.push_back({p.x + 1, p.y, p.z});
        }

        if (localBlockPostion.y == 0) {
            m_chunks.updates.push_back({p.x, p.y - 1, p.z});
        }
        else if (localBlockPostion.y == CHUNK_SIZE - 1) {
            m_chunks.updates.push_back({p.x, p.y + 1, p.z});
        }

        if (localBlockPostion.z == 0) {
            m_chunks.updates.push_back({p.x, p.y, p.z - 1});
        }
        else if (localBlockPostion.z == CHUNK_SIZE - 1) {
            m_chunks.updates.push_back({p.x, p.y, p.z + 1});
        }
    }
    m_chunks.blockUpdates.clear();

    auto playerChunk = worldToChunkPosition(mp_player->position);
    auto distanceToPlayer = [&playerChunk](const ChunkPosition &chunkPosition) {
        return glm::abs(playerChunk.x - chunkPosition.x) +
               glm::abs(playerChunk.y - chunkPosition.y) +
               glm::abs(playerChunk.z - chunkPosition.z);
    };

    if (!m_chunks.updates.empty()) {
        // Sort chunk updates by distance if the update vector is not
        // sorted already
        if (!std::is_sorted(m_chunks.updates.begin(), m_chunks.updates.end(),
                            [&](const auto &a, const auto &b) {
                                return distanceToPlayer(a) <
                                       distanceToPlayer(b);
                            })) {
            // Remove non-unique elements
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

        if (m_noMeshingCount != m_chunks.updates.size()) {
            m_blockMeshing = false;
        }

        // Find first "meshable" chunk
        int count = 0;
        if (!m_blockMeshing) {
            m_noMeshingCount = 0;
            for (auto itr = m_chunks.updates.cbegin();
                 itr != m_chunks.updates.cend();) {
                if (m_chunks.manager.hasNeighbours(*itr)) {
                    auto &chunk = m_chunks.manager.getChunk(*itr);
                    auto buffer = makeChunkMesh(chunk, m_voxelData);
                    m_chunks.bufferables.push_back(buffer);
                    deleteChunkRenderable(*itr);
                    itr = m_chunks.updates.erase(itr);

                    // Break so that the game still runs while world is
                    // being built
                    // TODO: Work out a way to make this concurrent (aka
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
                m_blockMeshing = true;
            }
        }
    }

    m_blockSelected = false;
    Ray ray(mp_player->position, mp_player->rotation);

    for (; ray.getLength() < 8; ray.step()) {
        auto rayBlockPosition = toBlockPosition(ray.getEndpoint());
        auto &voxel = m_voxelData.getVoxelData(
            m_chunks.manager.getBlock(rayBlockPosition));
        if (isVoxelSelectable(voxel.type)) {
            m_currentSelectedBlockPos = rayBlockPosition;
            m_blockSelected = true;
            break;
        }
    }

    // Call update function on the GUI script
    // Note: This part is quite dangerous, if there's no update() or there's an
    // error
    //       in the script then it will cause a crash
    // sol::function p_update = m_lua.lua["update"];
    // p_update(dt);
}

void Client::render(int width, int height)
{
    // TODO [Hopson] Clean this up
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

    for (auto &ent : m_entities) {

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
    }

    // Render chunks
    m_voxelTextures.bind();

    // Buffer chunks
    for (auto &chunkMesh : m_chunks.bufferables) {
        // TODO [Hopson] -> DRY this code
        if (chunkMesh.blockMesh.indicesCount > 0) {
            m_chunks.drawables.push_back({chunkMesh.blockMesh.position,
                                          chunkMesh.blockMesh.createBuffer()});
        }
        if (chunkMesh.fluidMesh.indicesCount > 0) {
            m_chunks.fluidDrawables.push_back(
                {chunkMesh.fluidMesh.position,
                 chunkMesh.fluidMesh.createBuffer()});
        }
    }
    m_chunks.bufferables.clear();

    // Render solid chunk blocks
    m_chunkShader.program.bind();
    gl::loadUniform(m_chunkShader.projectionViewLocation, playerProjectionView);

    renderChunks(m_chunks.drawables, m_frustum);

    glCheck(glEnable(GL_BLEND));

    // Render selection box
    if (m_blockSelected) {
        glCheck(glEnable(GL_LINE_SMOOTH));
        glCheck(glLineWidth(2.0));
        m_selectionShader.program.bind();
        glm::mat4 modelMatrix{1.0};
        float size = 1.005;
        translateMatrix(modelMatrix,
                        {m_currentSelectedBlockPos.x - (size - 1) / 2,
                         m_currentSelectedBlockPos.y - (size - 1) / 2,
                         m_currentSelectedBlockPos.z - (size - 1) / 2});
        scaleMatrix(modelMatrix, size);
        gl::loadUniform(m_selectionShader.modelLocation, modelMatrix);
        gl::loadUniform(m_selectionShader.projectionViewLocation,
                        playerProjectionView);
        m_selectionBox.getDrawable().bindAndDraw(GL_LINES);
    }

    // Render fluid mesh
    m_fluidShader.program.bind();
    gl::loadUniform(m_fluidShader.timeLocation,
                    m_clock.getElapsedTime().asSeconds());
    gl::loadUniform(m_fluidShader.projectionViewLocation, playerProjectionView);
    if (m_chunks.manager.getBlock(toBlockPosition(mp_player->position)) == 4) {
        glCheck(glCullFace(GL_FRONT));
    }
    renderChunks(m_chunks.fluidDrawables, m_frustum);
    glCheck(glDisable(GL_BLEND));
    glCheck(glCullFace(GL_BACK));

    // GUI
    m_gui.render(width, height);
}

void Client::endGame()
{
    NetworkHost::disconnectFromPeer(mp_serverPeer);
}

EngineStatus Client::currentStatus() const
{
    return m_status;
}

void Client::deleteChunkRenderable(const ChunkPosition &position)
{
    ::deleteChunkRenderable(position, m_chunks.drawables);
    ::deleteChunkRenderable(position, m_chunks.fluidDrawables);
}
