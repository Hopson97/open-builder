#include "playing_state.h"

#include "../gui/gui_system.h"
#include "../gui/widget/button_widget.h"

PlayingState::PlayingState(StateManager& manager)
    : ClientGameState(manager)
{
}

void PlayingState::input()
{
}

void PlayingState::update()
{
}

void PlayingState::render()
{
}

void PlayingState::onStart(gui::GuiSystem& gui)
{
    gui.changeGui("hud");
}

void PlayingState::onShutdown()
{
}
