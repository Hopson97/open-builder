#include "keyboard.h"

Keyboard::Keyboard()
{
    std::fill(m_keys.begin(), m_keys.end(), false);
}

void Keyboard::update(sf::Event& event)
{
    if (auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if ((size_t)key->code < m_keys.size())
        {
            m_keys[(size_t)key->code] = true;
            m_recentlyReleased = key->code;
        }
    }
    else if (auto* key = event.getIf<sf::Event::KeyReleased>())
    {
        if ((size_t)key->code < m_keys.size())
        {
            m_keys[(size_t)key->code] = false;
        }
    }
}

bool Keyboard::isKeyDown(sf::Keyboard::Key key) const
{
    return m_keys[(size_t)key];
}

bool Keyboard::keyReleased(sf::Keyboard::Key key) const
{
    return m_recentlyReleased == key;
}

void Keyboard::resetKeys()
{
    std::fill(m_keys.begin(), m_keys.end(), false);
}
