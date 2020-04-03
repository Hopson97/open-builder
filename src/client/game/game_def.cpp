#include "game_def.h"

bool ClientGameDef::start(const std::string ipAddress)
{
    auto connection = m_client.connectTo(ipAddress);
    if (!connection.success) {
        std::cout << "ERROR: " << connection.message << "\n";
        shutdown();
        return false;
    }
    return true;
}

void ClientGameDef::handleEvent(const sf::Event& event)
{
}

void ClientGameDef::handleInput(const Keyboard& keybaord, const InputState& inputState)
{
}

void ClientGameDef::tick(float dt)
{
    m_client.tick();
    if (m_client.getConnnectionState() == ConnectionState::Disconnected) {
        shutdown();
    }
}

void ClientGameDef::render()
{
}

void ClientGameDef::shutdown()
{
    m_client.disconnect();
    onShutdown();
}