#include "playing_state.h"

#include "../gui/gui_system.h"
#include "../gui/widget/button_widget.h"
#include <common/network/net_constants.h>

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

    m_localServer.runAsThread();
    m_client.connectTo(LOCAL_HOST);
}

void PlayingState::onShutdown()
{
    m_client.disconnect();
    m_localServer.stop();
}
