#include "game_def.h"

#include "../client_config.h"
#include "../input/input_state.h"
#include "../input/keyboard.h"
#include "../window.h"

bool ClientGameDef::start(const std::string ipAddress)
{
    auto connection = m_client.connectTo(ipAddress);
    if (!connection.success) {
        std::cout << "ERROR: " << connection.message << "\n";
        shutdown();
        return false;
    }
    m_client.setWorld(m_world);

    m_camera = Camera::createCamera();

    // TODO Move to the client/server handling
    m_world.setupData(1024);
    return true;
}

void ClientGameDef::shutdown()
{
    m_client.disconnect();
    onShutdown();
}

void ClientGameDef::handleEvent(const sf::Event& event)
{
}

void ClientGameDef::handleInput(const Keyboard& keyboard, const InputState& inputState)
{
    if (inputState.isMouseLocked) {
        handlePlayerInput(keyboard);
    }
}

void ClientGameDef::tick(float dt)
{
    m_camera.update(m_world.getPlayer());
    m_client.tick();
    m_world.tick(dt);
    if (m_client.getConnnectionState() == ConnectionState::Disconnected) {
        shutdown();
    }

    m_client.sendPlayerState(m_world.getPlayer());
}

void ClientGameDef::render()
{
    m_world.render(m_camera);
}

void ClientGameDef::handlePlayerInput(const Keyboard& keyboard)
{
    auto& ctx = *Window::context;
    static auto lastMousePosition = sf::Mouse::getPosition(ctx);

    auto& player = m_world.getPlayer();
    glm::vec3& rotation = player.rotation;
    glm::vec3& positiion = player.position;
    glm::vec3& velocity = player.velocity;

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