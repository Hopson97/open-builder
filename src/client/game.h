#pragma once

#include "client_config.h"
#include <memory>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/Window.hpp>
#include <server_engine.h>

#include "network/client.h"

class Keyboard;
struct InputState;

class Game {
  public:
    bool initGame(const std::string& ipAddress);
    bool initGame();

    void stopGame();

    // Ran during game loop
    void onMouseRelease(sf::Mouse::Button button);
    void input(sf::Window& window, const Keyboard& keyboard,
               const InputState& inputState);
    void update(float dt);
    void render();

  private:
    bool init(const std::string& ip);

    Client m_client;

    bool m_isInGame = false;

    std::unique_ptr<ServerLauncher> m_serverLauncher;
};
