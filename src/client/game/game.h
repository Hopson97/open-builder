#pragma once

#include <SFML/Window/Event.hpp>
#include <memory>
#include <server_engine.h>
#include "game_def.h"

class Game {
  public:
    template <typename T, typename... Args>
    void setGameDefintion(Args&&... args)
    {
        m_gameDef = std::make_unique<T>(std::forward<Args>(args)...);
    }

    Game();

    bool start();
    void shutdown();

    void handleEvent(const sf::Event& event);
    void handleInput(const Keyboard& keyboard, const InputState& inputState);
    void tick(float dt);
    void render();

  private:
    bool isInGame() const;
    std::unique_ptr<ClientGameDef> m_gameDef;
};
