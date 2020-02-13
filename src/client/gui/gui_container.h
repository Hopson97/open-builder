#pragma once

class GuiContainer {
  public:
    GuiContainer();

    void hide();
    void show();

    void render();

  private:
    static int uidCount;
    int m_uid;

    bool m_isHidden = true;
};
