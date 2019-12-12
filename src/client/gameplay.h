#pragma once

#include "gl/gl_object.h"
#include "maths.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <common/network/enet.h>
#include <common/network/packet.h>

class Keyboard;

struct Entity {
    glm::vec3 position{0.0f, 0.0f, 12.0f}, rotation{0.0f};
    bool active = false;
};

struct Client {
};

class Gameplay {
  public:
    bool init(float aspect);
    void handleInput(const sf::Window &window, const Keyboard &keyboard);
    void onKeyRelease(sf::Keyboard::Key key);

    void update();
    void render();
    void endGame();

  private:
    void onDataReceive(const ENetPacket &packet);

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

    Entity *m_player = nullptr;

    ENetHost *m_client = nullptr;
    ENetPeer *m_serverPeer = nullptr;
    client_id_t m_clientId = 0;

    bool m_isMouseLocked = false;
};