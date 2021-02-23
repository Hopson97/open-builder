#include "Input.h"

Keyboard::Keyboard()
{
    std::fill(m_keys.begin(), m_keys.end(), false);
}

bool Keyboard::isKeyDown(sf::Keyboard::Key key) const
{
    return m_keys[key];
}

void Keyboard::update(sf::Event e)
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

void Keyboard::resetKeys()
{
    std::fill(m_keys.begin(), m_keys.end(), false);
}

const float SPEED = 0.5f;
void FirstPersonController::control(const sf::Window& window, const Keyboard& keyboard,
                                    glm::vec3& position, glm::vec3& rotation)
{
    sf::Vector2i change = sf::Mouse::getPosition(window) - m_lastMousePosition;
    rotation.x += static_cast<float>(change.y / 8.0f * 0.5);
    rotation.y += static_cast<float>(change.x / 8.0f * 0.5);
    sf::Mouse::setPosition({(int)window.getSize().x / 2, (int)window.getSize().y / 2});
    m_lastMousePosition = sf::Mouse::getPosition(window);

    // Inout for keyboard
    if (keyboard.isKeyDown(sf::Keyboard::W)) {
        position += forwardsVector(rotation) * SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::S)) {
        position += backwardsVector(rotation) * SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::A)) {
        position += leftVector(rotation) * SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::D)) {
        position += rightVector(rotation) * SPEED;
    }
}
