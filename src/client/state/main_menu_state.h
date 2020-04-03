#pragma once

#include "state_manager.h"

class MainMenuGameState final : public ClientGameState {
  public:
    MainMenuGameState(StateManager& manager);

    void onStart(gui::GuiSystem& gui) final override;
    void onShutdown() final override;
};
