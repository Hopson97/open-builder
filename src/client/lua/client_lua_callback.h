#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <sol/sol.hpp>
#include <vector>

struct ScriptEngine;

class ClientLuaCallbacks {
  public:
    ClientLuaCallbacks(ScriptEngine& scriptEngine);

    void onClientStartup();
    void onKeyboardKeyReleased(sf::Keyboard::Key key);

  private:
    std::vector<sol::function> m_onClientStartupCallbacks;
    std::array<std::vector<sol::function>, sf::Keyboard::KeyCount>
        m_onKeyReleaseCallbacks;
};