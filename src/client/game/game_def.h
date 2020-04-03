#pragma once

#include <SFML/Window/Event.hpp>
#include "../network/client.h"

class Keyboard;
struct InputState;

class ClientGameDef {
  public:
    virtual ~ClientGameDef() = default;

    void handleEvent(const sf::Event& event);
    void handleInput(const Keyboard& keybaord, const InputState& inputState);
    void tick(float dt);
    void render();

    virtual bool start() = 0;

    void shutdown();

  protected:
    bool start(const std::string ipAddress);

  private:
    virtual void onShutdown() = 0;

    Client m_client;
};