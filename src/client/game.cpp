#include "game.h"

bool Game::initGame()
{
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
    auto result = m_client.connectTo(ip);
    if (!result.success) {
        std::cout << "ERROR: " << result.message << "\n";
        stopGame();
        return false;
    }
    m_isInGame = true;
    return true;
}

void Game::stopGame()
{
    m_client.disconnect();
    if (m_serverLauncher) {
        m_serverLauncher->stop();
        m_serverLauncher.release();
    }
}

void Game::onMouseRelease(sf::Mouse::Button button)
{
    if (m_isInGame) {
    }
}

void Game::input(sf::Window& window, const Keyboard& keyboard,
                 const InputState& inputState)
{
    if (m_isInGame) {
    }
}

void Game::update(float dt)
{
    if (m_isInGame) {
        m_client.tick();
    }
}

void Game::render()
{
    if (m_isInGame) {
    }
}