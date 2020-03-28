#pragma once

#include "gl/shader.h"
#include "gl/textures.h"
#include "gl/vertex_array.h"
#include "input/keyboard.h"
#include "maths.h"
#include "renderer/chunk_renderer.h"
#include <SFML/Window/Mouse.hpp>
#include <common/network/command_dispatcher.h>
#include <common/network/net_host.h>
#include <common/world/chunk_manager.h>
#include <common/world/voxel_data.h>
#include <common/world/world_constants.h>
#include <unordered_set>

class Keyboard;
struct InputState;

namespace gui {
    class LabelWidget;
}

struct VoxelUpdate {
    VoxelPosition position;
    voxel_t voxel = 0;
};

struct Entity final {
    glm::vec3 position{0.0f};
    glm::vec3 rotation{0.0f};
    glm::vec3 velocity{0.0f};
    bool active = false;

    gl::Texture2d playerSkin; // May need to be relocated to its own Player Entity
};

class Client final : public NetworkHost {
  public:
    Client();

    bool init(const std::string& ipAddress);
    void handleInput(const sf::Window& window, const Keyboard& keyboard,
                     const InputState& inputState);
    void onMouseRelease(sf::Mouse::Button button);

    void update(float dt);
    void render();
    void endGame();

  private:
    // Network functions; defined in the src/client/network/client_command.cpp
    // directory
    void sendPlayerPosition(const glm::vec3& position);
    void sendVoxelUpdate(const VoxelUpdate& update);
    void sendPlayerSkin(const sf::Image& playerSkin);

    void onPeerConnect(ENetPeer* peer) override;
    void onPeerDisconnect(ENetPeer* peer) override;
    void onPeerTimeout(ENetPeer* peer) override;
    void onCommandRecieve(ENetPeer* peer, sf::Packet& packet, command_t command) override;

    void onPlayerJoin(sf::Packet& packet);
    void onPlayerLeave(sf::Packet& packet);
    void onSnapshot(sf::Packet& packet);
    void onChunkData(sf::Packet& packet);
    void onSpawnPoint(sf::Packet& packet);
    void onVoxelUpdate(sf::Packet& packet);
    void onPlayerSkinReceive(sf::Packet& packet);

    void onGameRegistryData(sf::Packet& packet);
    // End of network functions

    // Network
    ENetPeer* mp_serverPeer = nullptr;
    CommandDispatcher<Client, ClientCommand> m_commandDispatcher;
    bool m_hasReceivedGameData = false;

    // Rendering/ OpenGL stuff
    ViewFrustum m_frustum{};
    glm::mat4 m_projectionMatrix{1.0f};

    gl::VertexArray m_cube;

    gl::VertexArray m_selectionBox;

    gl::Texture2d m_errorSkinTexture;
    sf::Image m_rawPlayerSkin;

    gl::TextureArray m_voxelTextures;

    ChunkRenderer m_chunkRenderer;

    struct {
        gl::Shader program;
        gl::UniformLocation modelLocation;
        gl::UniformLocation projectionViewLocation;
    } m_basicShader;

    struct {
        gl::Shader program;
        gl::UniformLocation modelLocation;
        gl::UniformLocation projectionViewLocation;
    } m_selectionShader;

    // For time-based render stuff eg waves in the water
    sf::Clock m_clock;

    // Gameplay/ World
    std::array<Entity, 512> m_entities;

    VoxelPosition m_currentSelectedVoxelPos;
    bool m_voxelSelected = false;
    Entity* mp_player = nullptr;

    struct {
        ChunkManager manager;
        std::vector<ChunkPosition> updates;
        std::vector<VoxelUpdate> voxelUpdates;
    } m_chunks;

    VoxelDataManager m_voxelData;

    unsigned m_noMeshingCount = 0;
    bool m_voxelMeshing = false;
};
