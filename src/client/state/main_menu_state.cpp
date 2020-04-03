#include "main_menu_state.h"

#include "../gui/gui_system.h"
#include "../gui/widget/button_widget.h"

MainMenuGameState::MainMenuGameState(StateManager& manager)
    : ClientGameState(manager)
{
}

void MainMenuGameState::onStart(gui::GuiSystem& gui)
{
    gui.changeGui("main_menu");
    gui.onGuiChange([&](auto& id, auto& overlay) {
        if (id == "main_menu") {
            auto button = overlay.findButton("exit_button");
            if (button) {
                button->setOnClick([&]() { mp_stateManager->pop(); });
            }
        }
    });
}

void MainMenuGameState::onShutdown()
{
}
