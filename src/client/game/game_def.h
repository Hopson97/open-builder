#pragma once

#include "../network/client.h"
#include <SFML/Window/Event.hpp>

#include "../gl/shader.h"
#include "../gl/textures.h"
#include "../gl/vertex_array.h"
#include "../renderer/camera.h"
#include "client_world.h"

class Keyboard;
struct InputState;

class ClientGameDef {
  public:
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
    void handlePlayerInput(const Keyboard& keyboard);

    virtual void onShutdown() = 0;

    ClientWorld m_world;

    Camera m_camera;

    Client m_client;
};