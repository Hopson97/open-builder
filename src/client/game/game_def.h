#pragma once

#include "../network/client.h"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "../gl/shader.h"
#include "../gl/textures.h"
#include "../gl/vertex_array.h"
#include "../renderer/camera.h"
#include "client_world.h"
#include "player.h"

class Keyboard;
struct InputState;

struct SelectedBoxRenderer {
    gl::Shader program;
    gl::UniformLocation modelLocation;
    gl::UniformLocation projectionViewLocation;
    gl::VertexArray m_selectionBox;

    void create();
    void render(const Camera& camera, const VoxelPosition& position);
};

class ClientGameDef {
  public:
    ClientGameDef();
    virtual ~ClientGameDef() = default;

    void handleEvent(const sf::Event& event);
    void handleInput(const Keyboard& keyboard, const InputState& inputState);
    void tick(float dt);
    void render();

    virtual bool start() = 0;

    void shutdown();

  protected:
    bool start(const std::string ipAddress);

  private:
    virtual void onShutdown() = 0;

    ClientWorld m_world;
    Camera m_camera;
    Client m_client;

    Player m_player;

    SelectedBoxRenderer m_selectionBoxRenderer;
    VoxelPosition m_currentSelectedVoxelPos;
    bool m_isVoxelSelected = false;

    sf::Clock m_timer;
    sf::Time m_lastTime;
};
