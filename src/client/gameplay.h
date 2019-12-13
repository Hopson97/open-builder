#pragma once

#include "gl/gl_object.h"
#include "maths.h"
#include <SFML/Network/Packet.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <common/network/enet.h>
#include <common/network/net_types.h>
#include <common/network/net_host.h>

class Keyboard;

struct Entity {
    glm::vec3 position{0.0f, 0.0f, 12.0f}, rotation{0.0f};
    bool active = false;
};

class Gameplay : public NetworkHost {
  public:
    Gameplay();

    bool init(float aspect);
    void handleInput(const sf::Window &window, const Keyboard &keyboard);
    void onKeyRelease(sf::Keyboard::Key key);

    void update();
    void render();
    void endGame();

  private:
    void onPeerConnect(ENetPeer &peer) override;
    void onPeerDisconnect(ENetPeer &peer) override;
    void onPeerTimeout(ENetPeer &peer) override;
    void onCommandRecieve(sf::Packet & packet, command_t command) override;

    void onPlayerJoin(sf::Packet &packet);
    void onPlayerLeave(sf::Packet &packet);
    void onSnapshot(sf::Packet &packet);

    glm::mat4 m_projectionMatrix{1.0f};

    gl::VertexArray m_cube;
    gl::Shader m_shader;
    gl::Texture2d m_texture;

    gl::UniformLocation m_modelLocation;
    gl::UniformLocation m_projectionViewLocation;

    std::array<Entity, 512> m_entities;

    Entity *mp_player = nullptr;
    ENetPeer *mp_serverPeer = nullptr;

    bool m_isMouseLocked = false;
};