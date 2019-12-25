#pragma once

#include "client_engine.h"
#include "client_state.h"
#include "gl/gl_object.h"
#include "maths.h"
#include "network/client.h"
#include "world/chunk_mesh.h"
#include <SFML/Network/Packet.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <common/network/enet.h>
#include <common/network/net_types.h>
#include <common/world/chunk.h>

class Keyboard;

class Gameplay final {
  public:
    Gameplay();

    bool init(float aspect);
    void handleInput(const sf::Window &window, const Keyboard &keyboard);
    void onKeyRelease(sf::Keyboard::Key key);

    void update();
    void render();
    void endGame();

    EngineStatus currentStatus() const;

  private:
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

    ClientState m_clientState;
    Client m_netClient;

    Entity *mp_player = nullptr;

    bool m_isMouseLocked = false;
};