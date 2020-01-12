#pragma once

#include "client_engine.h"
#include "gl/shader.h"
#include "gl/textures.h"
#include "gl/vertex_array.h"
#include "maths.h"
#include "world/chunk_mesh.h"
#include <SFML/Network/Packet.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <common/network/enet.h>
#include <common/network/net_host.h>
#include <common/network/net_types.h>
#include <common/world/chunk_manager.h>
#include <unordered_set>

class Keyboard;

struct BlockUpdate {
    BlockPosition position;
    block_t block = 0;
};

struct Entity final {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 velocity{0.0f};
    bool active = false;

    gl::Texture2d playerSkin; // May need to be relocated to its own Player Entity
};

struct ChunkDrawable {
    ChunkPosition position;
    gl::VertexArray vao;
};

class Client final : public NetworkHost {
  public:
    Client();

    bool init(const ClientConfig& config, float aspect);
    void handleInput(const sf::Window &window, const Keyboard &keyboard);
    void onKeyRelease(sf::Keyboard::Key key);
    void onMouseRelease(sf::Mouse::Button button, int x, int y);

    void update(float dt);
    void render();
    void endGame();

    EngineStatus currentStatus() const;

  private:
    // Network functions; defined in the src/client/network/client_command.cpp
    // directory
    void sendPlayerPosition(const glm::vec3 &position);
    void sendBlockUpdate(const BlockUpdate &update);
    void sendPlayerSkin(const sf::Image &playerSkin);

    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;
    void onPeerTimeout(ENetPeer *peer) override;
    void onCommandRecieve(ENetPeer *peer, sf::Packet &packet,
                          command_t command) override;

    void onPlayerJoin(sf::Packet &packet);
    void onPlayerLeave(sf::Packet &packet);
    void onSnapshot(sf::Packet &packet);
    void onChunkData(sf::Packet &packet);
    void onSpawnPoint(sf::Packet &packet);
    void onBlockUpdate(sf::Packet &packet);
    void onPlayerSkinReceive(sf::Packet &packet);
    // End of network functions

    int findChunkDrawableIndex(const ChunkPosition &position);
    void deleteChunkRenderable(const ChunkPosition &position);

    // Network
    ENetPeer *mp_serverPeer = nullptr;

    // Rendering/ OpenGL stuff
    glm::mat4 m_projectionMatrix{1.0f};

    gl::VertexArray m_cube;

    gl::Texture2d m_errorSkinTexture;
    sf::Image m_rawPlayerSkin;
    gl::TextureArray m_blockTextures;

    struct {
        gl::Shader program;
        gl::UniformLocation modelLocation;
        gl::UniformLocation projectionViewLocation;
    } m_basicShader;

    struct {
        gl::Shader program;
        gl::UniformLocation projectionViewLocation;
    } m_chunkShader;

    // Gameplay/ World
    std::array<Entity, 512> m_entities;

    Entity *mp_player = nullptr;

    struct {
        std::vector<ChunkMesh> bufferables;
        std::vector<ChunkDrawable> drawables;
        ChunkManager manager;
        std::vector<ChunkPosition> updates;
        std::vector<BlockUpdate> blockUpdates;
    } m_chunks;

    // Engine-y stuff
    EngineStatus m_status = EngineStatus::Ok;
    bool m_isMouseLocked = false;

    ViewFrustum m_frustum;

    unsigned m_noMeshingCount = 0;
    bool m_blockMeshing = false;
};