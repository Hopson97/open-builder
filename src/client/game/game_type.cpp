#include "game_type.h"

#include <common/network/net_constants.h>

/// Local game
LocalGame::LocalGame(const std::string& worldName)
    : m_worldName(worldName)
{
}

bool LocalGame::start()
{
    m_serverLauncher.runAsThread();
    return ClientGameDef::start(LOCAL_HOST);
}

void LocalGame::onShutdown()
{
    m_serverLauncher.stop();
}

/// Remote game
RemoteGame::RemoteGame(const std::string& ipAddress)
    : m_serverIp(ipAddress)
{
}

bool RemoteGame::start()
{
    return ClientGameDef::start(m_serverIp);
}

void RemoteGame::onShutdown()
{
    // empty
}
