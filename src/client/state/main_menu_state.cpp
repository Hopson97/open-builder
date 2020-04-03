#include "main_menu_state.h"

#include "../gui/gui_system.h"
#include "../gui/widget/button_widget.h"
#include "playing_state.h"

MainMenuGameState::MainMenuGameState(StateManager& manager)
    : ClientGameState(manager)
{
}

void MainMenuGameState::onStart(gui::GuiSystem& gui)
{
    gui.changeGui("main_menu");
    gui.onGuiChange([&](auto& id, auto& overlay) {
        if (id == "main_menu") {
            auto exit = overlay.findButton("exit_button");
            if (exit) {
                exit->setOnClick([&]() { mp_stateManager->pop(); });
            }
       
            auto cont = overlay.findButton("continue_game");
            if (cont) {
                cont->setOnClick([&]() {
                    mp_stateManager->push(
                        gui, std::make_unique<PlayingState>(*mp_stateManager));
                });
            }
        }
    });
}

void MainMenuGameState::onShutdown()
{
}
