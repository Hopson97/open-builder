#pragma once

#include "../Maths.h"
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <array>

class Keyboard {
  public:
    Keyboard();
    bool isKeyDown(sf::Keyboard::Key key) const;
    void update(sf::Event e);

  private:
    void resetKeys();

    std::array<bool, sf::Keyboard::KeyCount> m_keys;
};

class FirstPersonController {
  public:
    void control(const sf::Window& window, const Keyboard& keyboard, glm::vec3& position,
                 glm::vec3& rotation);

  private:
    sf::Vector2i m_lastMousePosition;
};