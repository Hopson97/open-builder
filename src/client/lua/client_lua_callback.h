#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <sol/sol.hpp>
#include <vector>

struct ScriptEngine;

class ClientLuaCallbacks {
  public:
    void initCallbacks(ScriptEngine& scriptEngine);

    void onClientStartup();
    void onEnterGame();
    void onExitGame();
    void onError(const std::string& error);
    void onKeyboardKeyReleased(sf::Keyboard::Key key);

  private:
    std::vector<sol::function> m_onClientStartupCallbacks;
    std::vector<sol::function> m_onEnterGameCallbacks;
    std::vector<sol::function> m_onExitGameCallbacks;
    std::vector<sol::function> m_onErrorCallbacks;

    std::array<std::vector<sol::function>, sf::Keyboard::KeyCount>
        m_onKeyReleaseCallbacks;
};