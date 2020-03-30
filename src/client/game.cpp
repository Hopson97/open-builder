#include "game.h"

bool Game::initGame()
{
    // Client.serverIp = LOCAL_HOST;

    m_serverLauncher = std::make_unique<ServerLauncher>(sf::milliseconds(1000));
    m_serverLauncher->runAsThread();

    return init(LOCAL_HOST);
}

bool Game::initGame(const std::string& ipAddress)
{
    return init(ipAddress);
}

bool Game::init(const std::string& ip)
{
    auto result = m_netClient.connectTo(ip);
    if (!result.success) {
        std::cout << "ERROR: " << result.message << "\n";
        return false;
    }

    m_client = std::make_unique<Client>();
    if (!m_client->init(ip)) {
        stopGame();
        return false;
    }
    return true;
}

void Game::stopGame()
{
    if (m_client) {
        m_netClient.disconnect();
        m_client->endGame();
        m_client->destroy();
        m_client.release();
    }
    if (m_serverLauncher) {
        m_serverLauncher->stop();
        m_serverLauncher.release();
    }
}

void Game::onMouseRelease(sf::Mouse::Button button)
{
    if (m_client) {
        m_client->onMouseRelease(button);
    }
}

void Game::input(sf::Window& window, const Keyboard& keyboard,
                 const InputState& inputState)
{
    if (m_client) {
        m_client->handleInput(window, keyboard, inputState);
    }
}

void Game::update(float dt)
{
    if (m_serverLauncher) {
        m_netClient.tick();
        // m_client->update(dt);
    }
}

void Game::render()
{
    if (m_client) {
        m_client->render();
    }
}