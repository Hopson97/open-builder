#include "game.h"

bool Game::initGame(ClientConfig config)
{
    config.serverIp = LOCAL_HOST;

    m_serverLauncher =
        std::make_unique<ServerLauncher>(ServerConfig{8, 8}, sf::milliseconds(100));
    m_serverLauncher->runAsThread();

    return init(config);
}

bool Game::initGame(ClientConfig config, const std::string& ipAddress)
{
    config.serverIp = ipAddress;

    return init(config);
}

bool Game::init(ClientConfig config)
{
    m_client = std::make_unique<Client>();
    if (!m_client->init(config, (float)config.windowWidth / (float)config.windowHeight)) {
        stopGame();
        return false;
    }
    return true;
}

void Game::stopGame()
{
    if (m_serverLauncher) {
        m_serverLauncher->stop();
        m_serverLauncher.release();
    }
    if (m_client) {
        m_client->endGame();
        m_client->destroy();
        m_client.release();
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
    if (m_client) {
        m_client->update(dt);
    }
}

void Game::render()
{
    if (m_client) {
        m_client->render();
    }
}