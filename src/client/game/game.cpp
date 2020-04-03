#include "game.h"

bool Game::start()
{
    if (isInGame()) {
        return m_gameDef->start();
    }
    return false;
}

void Game::shutdown()
{
    if (isInGame()) {
        m_gameDef->shutdown();
    }
}

void Game::handleEvent(const sf::Event& event)
{
    if (isInGame()) {
        m_gameDef->handleEvent(event);
    }
}

void Game::handleInput(const Keyboard& keyboard, const InputState& inputState)
{
    if (isInGame()) {
        m_gameDef->handleInput(keyboard, inputState);
    }
}

void Game::tick(float dt)
{
    if (isInGame()) {
        m_gameDef->tick(dt);
    }
}

void Game::render()
{
    if (isInGame()) {
        m_gameDef->render();
    }
}

bool Game::isInGame() const
{
    return m_gameDef != nullptr;
}
