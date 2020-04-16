#include "player.h"

#include "../client_config.h"
#include "../input/keyboard.h"
#include "../window.h"
#include "client_world.h"

void Player::input(const Keyboard& keyboard)
{
    mouseInput();
    keyboardInput(keyboard);
}

void Player::tick(ClientWorld& world, float dt)
{
    auto& position = m_state.position;
    auto& velocity = m_state.velocity;

    velocity += m_acceleration;
    m_acceleration = {0, 0, 0};

    if (!m_isOnGround) {
        velocity.y -= 20 * dt;
    }
    m_isOnGround = false;

    position.x += velocity.x * dt;
    collide(world, {velocity.x * dt, 0, 0});

    position.y += velocity.y * dt;
    collide(world, {0, velocity.y * dt, 0});

    position.z += velocity.z * dt;
    collide(world, {0, 0, velocity.z * dt});

    velocity.x *= 0.90f;
    velocity.z *= 0.90f;
}

struct {
    glm::vec3 dimensions{0.3f, 1.5f, 0.3f};
} box;

void Player::collide(const ClientWorld& world, const glm::vec3& vel)
{
    auto& position = m_state.position;
    auto& velocity = m_state.velocity;
    for (int x = position.x - box.dimensions.x; x < position.x + box.dimensions.x; x++) {
        for (int y = position.y - box.dimensions.y; y < position.y + 0.7; y++) {
            for (int z = position.z - box.dimensions.z; z < position.z + box.dimensions.z;
                 z++) {
                auto voxel = world.getVoxel(x, y, z);

                if (voxel.id != 0 && voxel.isCollidable) {
                    if (vel.y > 0) {
                        position.y = y - box.dimensions.y;
                        velocity.y = 0;
                    }
                    else if (vel.y < 0) {
                        m_isOnGround = true;
                        position.y = y + box.dimensions.y + 1;
                        velocity.y = 0;
                    }

                    if (vel.x > 0) {
                        position.x = x - box.dimensions.x;
                    }
                    else if (vel.x < 0) {
                        position.x = x + box.dimensions.x + 1;
                    }

                    if (vel.z > 0) {
                        position.z = z - box.dimensions.z;
                    }
                    else if (vel.z < 0) {
                        position.z = z + box.dimensions.z + 1;
                    }
                }
            }
        }
    }
}

void Player::keyboardInput(const Keyboard& keyboard)
{
    float PLAYER_SPEED = 0.4f;
    if (keyboard.isKeyDown(sf::Keyboard::LControl)) {
        PLAYER_SPEED *= 10;
    }

    glm::vec3& rotation = m_state.rotation;
    glm::vec3& velocity = m_state.velocity;

    if (keyboard.isKeyDown(sf::Keyboard::W)) {
        //m_acceleration += forwardsVector(rotation) * PLAYER_SPEED;
        m_acceleration.x += -glm::cos(glm::radians(rotation.y + 90)) * PLAYER_SPEED;
        m_acceleration.z += -glm::sin(glm::radians(rotation.y + 90)) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::S)) {
        //m_acceleration += backwardsVector(rotation) * PLAYER_SPEED;
        m_acceleration.x += glm::cos(glm::radians(rotation.y + 90)) * PLAYER_SPEED;
        m_acceleration.z += glm::sin(glm::radians(rotation.y + 90)) * PLAYER_SPEED;
    }
    if (keyboard.isKeyDown(sf::Keyboard::A)) {
        m_acceleration += leftVector(rotation) * PLAYER_SPEED;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::D)) {
        m_acceleration += rightVector(rotation) * PLAYER_SPEED;
    }

    if (keyboard.isKeyDown(sf::Keyboard::Space) && m_isOnGround) {
        m_acceleration.y += PLAYER_SPEED * 20;
    }
    else if (keyboard.isKeyDown(sf::Keyboard::LShift) && m_isFlying) {
        m_acceleration.y -= PLAYER_SPEED * 2;
    }
    if (rotation.x < -80.0f) {
        rotation.x = -79.9f;
    }
    else if (rotation.x > 85.0f) {
        rotation.x = 84.9f;
    }
}

void Player::mouseInput()
{
    auto& ctx = *Window::context;
    static auto lastMousePosition = sf::Mouse::getPosition(ctx);

    glm::vec3& rotation = m_state.rotation;

    float verticalSensitivity = ClientConfig::get().verticalSensitivity;
    float horizontalSensitivity = ClientConfig::get().horizontalSensitivity;
    auto change = sf::Mouse::getPosition(ctx) - lastMousePosition;
    rotation.x += static_cast<float>(change.y / 8.0f * verticalSensitivity);
    rotation.y += static_cast<float>(change.x / 8.0f * horizontalSensitivity);
    sf::Mouse::setPosition({(int)ctx.getSize().x / 2, (int)ctx.getSize().y / 2}, ctx);

// This fixes mouse jittering on mac
#ifndef __APPLE__
    lastMousePosition = sf::Mouse::getPosition(ctx);
#else
    lastMousePosition.x = (int)ctx.getSize().x / 2;
    lastMousePosition.y = (int)ctx.getSize().y / 2;
#endif
}
