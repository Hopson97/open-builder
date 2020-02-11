#pragma once

#include "client_engine.h"
#include "gl/shader.h"
#include "gl/textures.h"
#include "gl/vertex_array.h"
#include "gui/gui.h"
#include "gui/text.h"
#include "maths.h"
#include "world/chunk_mesh.h"
#include <SFML/Network/Packet.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <common/network/command_dispatcher.h>
#include <common/network/enet.h>
#include <common/network/net_host.h>
#include <common/network/net_types.h>
#include <common/scripting/script_engine.h>
#include <common/world/chunk_manager.h>
#include <common/world/voxel_data.h>
#include <unordered_set>

#include "renderer/chunk_renderer.h"

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

struct DebugStats {
    float fps = 0;
    float frameTime = 0;
    int renderedChunks = 0;
    size_t bytesRendered = 0;
};

class Client final : public NetworkHost {
  public:
    Client(const ClientConfig& config);

    bool init(const ClientConfig& config, float aspect);
    void handleInput(const sf::Window& window, const Keyboard& keyboard);
    void onKeyRelease(sf::Keyboard::Key key);
    void onMouseRelease(sf::Mouse::Button button, int x, int y);

    void update(float dt, float frameTime, float fps);
    void render(int width, int height);
    void endGame();

    EngineStatus currentStatus() const;

  private:
    // Network functions; defined in the src/client/network/client_command.cpp
    // directory
    void sendPlayerPosition(const glm::vec3& position);
    void sendBlockUpdate(const BlockUpdate& update);
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
    void onBlockUpdate(sf::Packet& packet);
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

    std::string m_texturePack;
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

    BlockPosition m_currentSelectedBlockPos;
    bool m_blockSelected = false;

    Entity* mp_player = nullptr;
    Entity m_externalCamera;

    struct {
        ChunkManager manager;
        std::unordered_set<ChunkPosition, ChunkPositionHash> updates;
        std::vector<BlockUpdate> blockUpdates;
    } m_chunks;

    VoxelDataManager m_voxelData;

    // Lua
    ScriptEngine m_lua;

    // GUI
    Gui m_gui;

    // Debug stats stuff
    DebugStats m_debugStats{};
    Text m_debugText;
    Font m_debugTextFont;
    sf::Clock m_debugTextUpdateTimer;
    bool m_shouldRenderDebugInfo = false;

    // Engine-y stuff
    EngineStatus m_status = EngineStatus::Ok;
    bool m_isMouseLocked = false;
};
