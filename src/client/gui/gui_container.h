#pragma once

class GuiContainer final {
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
