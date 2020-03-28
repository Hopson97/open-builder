#pragma once

#include "client.h"
#include "client_config.h"
#include <memory>
#include <server_engine.h>

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

    std::unique_ptr<Client> m_client;
    std::unique_ptr<ServerLauncher> m_serverLauncher;
};
