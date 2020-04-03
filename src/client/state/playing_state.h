#pragma once

#include "../network/client.h"
#include <server_engine.h>
#include "state_manager.h"

class PlayingState final : public ClientGameState {
  public:
    PlayingState(StateManager& manager);

    void input() final override;
    void update() final override;
    void render() final override;

    void onStart(gui::GuiSystem& gui) final override;
    void onShutdown() final override;

  private:
    ServerLauncher m_localServer;
    Client m_client;
};
#pragma once
