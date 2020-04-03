#pragma once

#include "state_manager.h"

class PlayingState final : public ClientGameState {
  public:
    PlayingState(StateManager& manager);

    void input() final override;
    void update() final override;
    void render() final override;

    void onStart(gui::GuiSystem& gui) final override;
    void onShutdown() final override;
};
#pragma once
