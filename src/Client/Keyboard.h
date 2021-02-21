#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>

class Keyboard {
  public:
    Keyboard()
    {
        std::fill(m_keys.begin(), m_keys.end(), false);
    }

    bool isKeyDown(sf::Keyboard::Key key) const
    {
        return m_keys[key];
    }

    void update(sf::Event e)
    {
        switch (e.type) {
            case sf::Event::KeyPressed:
                m_keys[e.key.code] = true;
                break;

            case sf::Event::KeyReleased:
                m_keys[e.key.code] = false;

            default:
                break;
        }
    }

  private:
    void resetKeys()
    {
        std::fill(m_keys.begin(), m_keys.end(), false);
    }

    std::array<bool, sf::Keyboard::KeyCount> m_keys;
};