#pragma once

#include "game_def.h"
#include <server_engine.h>

class LocalGame final : public ClientGameDef {
  public:
    LocalGame(const std::string& worldName);

    bool start() final override;

  private:
    void onShutdown() final override;

    const std::string m_worldName;
    ServerEngine m_serverLauncher;
};

class RemoteGame final : public ClientGameDef {
  public:
    RemoteGame(const std::string& ipAddress);

    bool start() final override;

  private:
    void onShutdown() final override;
    const std::string m_serverIp;
};
