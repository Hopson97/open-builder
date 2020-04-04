#include "game_def.h"
//
#include "../client_config.h"
#include "../input/input_state.h"
#include "../input/keyboard.h"
#include "../window.h"

namespace {
    void mouseInput(glm::vec3& rotation)
    {
        auto& ctx = *Window::context;
        static auto lastMousePosition = sf::Mouse::getPosition(ctx);

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
        lastMousePosition.x = (int)window.getSize().x / 2;
        lastMousePosition.y = (int)window.getSize().y / 2;
#endif
    }

    void keyboardInput(const Keyboard& keyboard, glm::vec3& rotation,
                       glm::vec3& velocity)
    {
        // Handle keyboard input
        float PLAYER_SPEED = 5.0f;
        if (keyboard.isKeyDown(sf::Keyboard::LControl)) {
            PLAYER_SPEED *= 10;
        }

        if (keyboard.isKeyDown(sf::Keyboard::W)) {
            velocity += forwardsVector(rotation) * PLAYER_SPEED;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::S)) {
            velocity += backwardsVector(rotation) * PLAYER_SPEED;
        }
        if (keyboard.isKeyDown(sf::Keyboard::A)) {
            velocity += leftVector(rotation) * PLAYER_SPEED;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::D)) {
            velocity += rightVector(rotation) * PLAYER_SPEED;
        }
        if (keyboard.isKeyDown(sf::Keyboard::Space)) {
            velocity.y += PLAYER_SPEED * 2;
        }
        else if (keyboard.isKeyDown(sf::Keyboard::LShift)) {
            velocity.y -= PLAYER_SPEED * 2;
        }
        if (rotation.x < -80.0f) {
            rotation.x = -79.9f;
        }
        else if (rotation.x > 85.0f) {
            rotation.x = 84.9f;
        }
    }

} // namespace

void handleFpsCameraInput(glm::vec3& rotation, glm::vec3& velocity,
                          const Keyboard& keyboard, const InputState& inputState)
{
    if (inputState.isMouseLocked) {
        mouseInput(rotation);
        keyboardInput(keyboard, rotation, velocity);
    }
}