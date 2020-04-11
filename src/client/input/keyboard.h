
#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>

/**
 * Holds the state of the keyboard
 */
class Keyboard final {
  public:
    Keyboard();

    /**
     * @brief Updates the state of the keyboard. Must be called every frame
     * in the event loop.
     *
     * @param e The event to check key events for
     */
    void update(sf::Event e);

    /**
     * @brief Check if a key is currently down
     *
     * @param key The key to check
     * @return true The key is down
     * @return false The key is up
     */
    bool isKeyDown(sf::Keyboard::Key key) const;

    /**
     * @brief Check if a key was recently released
     *
     * @param key The key to check if it was released
     * @return true The key was recently released
     * @return false The key was not recently released
     */
    bool keyReleased(sf::Keyboard::Key key) const;

  private:
    /**
     * @brief Resets keys back to the "unpressed" state
     *
     */
    void resetKeys();

    std::array<bool, sf::Keyboard::KeyCount> m_keys;
    sf::Keyboard::Key m_recentlyReleased;
};
