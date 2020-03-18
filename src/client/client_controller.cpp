#include "client_controller.h"

void ClientStateControl::createWorld(const std::string& name, const std::string& seed)
{
    if (currentState == StateId::InMenu) {
        currentState = StateId::CreateGame;
        paramA = name;
        paramB = seed;
    }
}

void ClientStateControl::loadWorld(const std::string& name)
{
    if (currentState == StateId::InMenu) {
        currentState = StateId::LoadGame;
        paramA = name;
    }
}

void ClientStateControl::joinWorld(const std::string& ipAddress)
{
    if (currentState == StateId::InMenu) {
        currentState = StateId::JoinGame;
        paramA = ipAddress;
    }
}

void ClientStateControl::pauseGame()
{
    if (currentState == StateId::InGame) {
        currentState = StateId::Paused;
    }
}

void ClientStateControl::resumeGame()
{
    if (currentState == StateId::Paused) {
        currentState = StateId::InGame;
    }
}

void ClientStateControl::exitGame()
{
    if (currentState == StateId::Paused || currentState == StateId::InGame) {
        currentState = StateId::ExitGame;
    }
}

void ClientStateControl::shutdown()
{
    currentState = StateId::Shutdown;
}