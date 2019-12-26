#pragma once

#include "client_engine.h"
#include "gl/gl_object.h"
#include "maths.h"
#include "world/chunk_mesh.h"
#include "world/client_chunk_manager.h"
#include <SFML/Network/Packet.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <common/network/enet.h>
#include <common/network/net_host.h>
#include <common/network/net_types.h>

class Keyboard;

struct Entity final {
    glm::vec3 position{0.0f, 0.0f, 12.0f}, rotation{0.0f};
    bool active = false;
};

class Client final : public NetworkHost {
  public:
    Client();

    bool init(float aspect);
    void handleInput(const sf::Window &window, const Keyboard &keyboard);
    void onKeyRelease(sf::Keyboard::Key key);

    void update();
    void render();
    void endGame();

    EngineStatus currentStatus() const;

  private:
    void sendDisconnectRequest();
    void sendPlayerPosition(const glm::vec3 &position);
    void sendChunkRequest(const ChunkPosition &position);

    void onPeerConnect(ENetPeer *peer) override;
    void onPeerDisconnect(ENetPeer *peer) override;
    void onPeerTimeout(ENetPeer *peer) override;
    void onCommandRecieve(ENetPeer *peer, sf::Packet &packet,
                          command_t command) override;

    void onPlayerJoin(sf::Packet &packet);
    void onPlayerLeave(sf::Packet &packet);
    void onSnapshot(sf::Packet &packet);
    void onChunkData(sf::Packet &packet);

    // Network
    ENetPeer *mp_serverPeer = nullptr;

    // Rendering/ OpenGL stuff
    glm::mat4 m_projectionMatrix{1.0f};

    gl::VertexArray m_cube;
    gl::Texture2d m_texture;
    gl::Texture2d m_grassTexture;

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
    ClientChunkManager m_chunkManager;

    Entity *mp_player = nullptr;

    // Engine-y stuff
    EngineStatus m_status = EngineStatus::Ok;
    bool m_isMouseLocked = false;
};