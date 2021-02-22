#pragma once

#include <functional>

struct ClientSettings {
  private:
    ClientSettings();
    ~ClientSettings();

  public:
    static ClientSettings& get()
    {
        static ClientSettings settings;
        return settings;
    }
    bool showFps = false;

    void showSettingsMenu(std::function<void(void)> onBackPressed);
};
