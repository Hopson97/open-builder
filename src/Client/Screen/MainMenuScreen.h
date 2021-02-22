#pragma once

#include "Screen.h"

class MainMenuScreen final : public Screen {
    enum class Menu {
        MainMenuScreen,
        SettingsMenu,
    };

  public:
    MainMenuScreen(ScreenManager& stack);

    void onRender() override;

  private:
    Menu m_activeMenu = Menu::MainMenuScreen;
};
