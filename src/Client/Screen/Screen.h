#pragma once

#include <SFML/Window/Window.hpp>
#include <memory>
#include <stack>
#include <vector>

class Screen;
class Keyboard;

class ScreenManager final {
    struct Action {
        enum class Kind {
            Push,
            Pop,
        };
        Kind kind;
        std::unique_ptr<Screen> screen;
    };

  public:
    void pushScreen(std::unique_ptr<Screen> screen);
    void popScreen();
    void update();
    Screen& peekScreen();
    bool hasScreen() const;

    //  private:
    std::stack<std::unique_ptr<Screen>> m_screens;
    std::vector<Action> m_actions;
};

// clang-format off
class Screen {
  public:
    Screen(ScreenManager& screens);
    virtual ~Screen() = default;

    virtual void onInput(const sf::Window&, const Keyboard&)  {}
    virtual void onUpdate([[maybe_unused]] float dt) {}
    virtual void onRender() = 0;

  protected:
    ScreenManager* m_pScreens;
};
// clang-format on

// Common and Handy ImGUI stuff for screens
bool imguiBeginCustom(const char* name);
bool imguiButtonCustom(const char* text);