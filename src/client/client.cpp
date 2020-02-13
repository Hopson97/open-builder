#include "client.h"

#include "gl/gl_errors.h"
#include "gl/primitive.h"
#include "input/keyboard.h"
#include "lua/client_lua_api.h"
#include "world/chunk_mesh_generation.h"
#include <SFML/Window/Mouse.hpp>
#include <common/debug.h>
#include <common/network/net_command.h>
#include <common/network/net_constants.h>
#include <iomanip>
#include <numeric>
#include <thread>

#include "client_config.h"

namespace {
bool isVoxelSelectable(VoxelType voxelType)
{
    return voxelType == VoxelType::Solid || voxelType == VoxelType::Flora;
}
} // namespace

Client::Client(const ClientConfig& config)
    : NetworkHost("Client")
    , m_guiMaster(config.windowWidth, config.windowHeight)
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

    initGuiApi(m_lua, m_guiMaster);

    m_lua.runLuaFile("game/client/main.lua");
}

bool Client::init(const ClientConfig& config, float aspect)
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
    m_errorSkinTexture.create("res/skins/error.png");
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

    // Font and text
    m_debugTextFont.init("res/VeraMono-Bold.ttf", 512);
    m_debugText.setPosition({2, config.windowHeight - 16, 0});
    m_debugText.setCharSize(16);
    m_debugText.setFont(m_debugTextFont);
    m_debugText.setText("Current FPS: 60");
    return true;
}

void Client::handleInput(const sf::Window& window, const Keyboard& keyboard)
{
    if (!m_hasReceivedGameData) {
        return;
    }
    static auto lastMousePosition = sf::Mouse::getPosition(window);

    if (!m_isMouseLocked && window.hasFocus() && sf::Mouse::getPosition(window).y >= 0) {
        auto change = sf::Mouse::getPosition(window) - lastMousePosition;
        mp_player->rotation.x += static_cast<float>(change.y / 8.0f);
        mp_player->rotation.y += static_cast<float>(change.x / 8.0f);
        sf::Mouse::setPosition({(int)window.getSize().x / 2, (int)window.getSize().y / 2},
                               window);
        lastMousePosition = sf::Mouse::getPosition(window);
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

void Client::onMouseRelease(sf::Mouse::Button button, [[maybe_unused]] int x,
                            [[maybe_unused]] int y)
{
    // Handle block removal/ block placing events

    auto blocks =
        getIntersectedBlocks(mp_player->position, forwardsVector(mp_player->rotation), 8);

    BlockPosition& previous = blocks.at(0);
    for (auto& position : blocks) {
        auto& voxel = m_voxelData.getVoxelData(m_chunks.manager.getBlock(position));

        if (isVoxelSelectable(voxel.type)) {
            BlockUpdate blockUpdate;
            blockUpdate.block = button == sf::Mouse::Left ? 0 : 1;
            if (button == sf::Mouse::Left) {
                blockUpdate.position = position;
            }
            else if (previous == toBlockPosition(mp_player->position)) {
                // prevents players from replacing blocks they're inside of
                break;
            }
            else {
                blockUpdate.position = previous;
            }
            blockUpdate.position = button == sf::Mouse::Left ? position : previous;
            m_chunks.blockUpdates.push_back(blockUpdate);
            sendBlockUpdate(blockUpdate);
            break;
        }
        previous = position;
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

        case sf::Keyboard::F3:
            m_shouldRenderDebugInfo = !m_shouldRenderDebugInfo;
            break;

        default:
            break;
    }
}

void Client::update(float dt, float frameTime)
{
    m_debugStats.frameTime = frameTime;

    NetworkHost::tick();
    if (!m_hasReceivedGameData) {
        return;
    }

    mp_player->position += mp_player->velocity * dt;
    mp_player->velocity *= 0.99 * dt;

    sendPlayerPosition(mp_player->position);

    // Update blocks
    for (auto& blockUpdate : m_chunks.blockUpdates) {
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
            m_blockMeshing = false;
        }

        // Find first "meshable" chunk
        int count = 0;
        if (!m_blockMeshing) {
            m_noMeshingCount = 0;
            for (auto itr = m_chunks.updates.cbegin(); itr != m_chunks.updates.cend();) {
                if (m_chunks.manager.hasNeighbours(*itr)) {
                    auto& chunk = m_chunks.manager.getChunk(*itr);

                    auto buffer = makeChunkMesh(chunk, m_voxelData);
                    m_chunkRenderer.updateMesh(*itr, std::move(buffer));
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

    // Determine if a player is selecting a block & if so, which
    m_blockSelected = false;
    auto blocks =
        getIntersectedBlocks(mp_player->position, forwardsVector(mp_player->rotation), 8);
    for (auto& position : blocks) {
        auto& voxel = m_voxelData.getVoxelData(m_chunks.manager.getBlock(position));
        if (isVoxelSelectable(voxel.type)) {
            m_currentSelectedBlockPos = position;
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
        m_chunks.manager.getBlock(toBlockPosition(mp_player->position)) ==
        m_voxelData.getVoxelId(CommonVoxel::Water);
    auto result = m_chunkRenderer.renderChunks(mp_player->position, m_frustum,
                                               playerProjectionView, isPlayerInWater);
    m_debugStats.bytesRendered = result.bytesInView;
    m_debugStats.renderedChunks = result.chunksRendered;

    // Render selection box
    if (m_blockSelected) {
        glCheck(glEnable(GL_BLEND));
        glCheck(glEnable(GL_LINE_SMOOTH));
        glCheck(glLineWidth(2.0));
        m_selectionShader.program.bind();
        glm::mat4 modelMatrix{1.0};
        float size = 1.005f;
        translateMatrix(modelMatrix, {m_currentSelectedBlockPos.x - (size - 1) / 2,
                                      m_currentSelectedBlockPos.y - (size - 1) / 2,
                                      m_currentSelectedBlockPos.z - (size - 1) / 2});
        scaleMatrix(modelMatrix, size);
        gl::loadUniform(m_selectionShader.modelLocation, modelMatrix);
        gl::loadUniform(m_selectionShader.projectionViewLocation, playerProjectionView);
        m_selectionBox.getDrawable().bindAndDraw(GL_LINES);
        glCheck(glDisable(GL_BLEND));
    }

    // GUI
    m_guiMaster.render();

    // Debug stats
    if (m_shouldRenderDebugInfo) {

        if (m_debugTextUpdateTimer.getElapsedTime() > sf::milliseconds(100)) {
            m_debugTextUpdateTimer.restart();

            // Get vertices total size in view and convert to MB
            size_t buffSize = m_chunkRenderer.getTotalBufferSize();
            buffSize /= 0x100000;
            m_debugStats.bytesRendered /= 0x100000;

            DebugStats& d = m_debugStats;
            glm::vec3& p = mp_player->position;
            glm::vec3& r = mp_player->rotation;
            auto bp = toLocalBlockPosition(p.x, p.y, p.z);
            auto cp = toChunkPosition(p.x, p.y, p.z);

            std::ostringstream debugText;
            debugText << "Frame time: " << std::setprecision(3) << d.frameTime << "ms\n";
            debugText << "Chunks: " << d.renderedChunks << " of "
                      << m_chunkRenderer.getTotalChunks() << " drawn\n";
            debugText << "Chunk VRAM: " << m_debugStats.bytesRendered << "Mb of "
                      << buffSize << "Mb drawn\n";
            debugText << "Position: " << p << '\n';
            debugText << "Chunk Position: " << cp << '\n';
            debugText << "Local Position: " << bp << '\n';
            debugText << "Rotation: " << r << '\n';
            debugText << "In Chunk? " << (m_chunks.manager.hasChunk(cp) ? "Yes" : "No")
                      << '\n';

            m_debugText.setText(debugText.str());
        }
        // m_gui.renderText(m_debugText);
    }
}

void Client::endGame()
{
    NetworkHost::disconnectFromPeer(mp_serverPeer);
}

EngineStatus Client::currentStatus() const
{
    return m_status;
}
