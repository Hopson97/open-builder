#pragma once

#include "client_state_controller.h"
#include "game/game.h"
#include "gui/gui_system.h"
#include "input/input_state.h"
#include "input/keyboard.h"
#include "lua/client_lua_callback.h"
#include "renderer/gui_renderer.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Window.hpp>
#include <common/lua/script_engine.h>

struct FPSCounter final {
    sf::Clock timer;
    float frameTime = 0;
    float frameCount = 0;

    void update()
    {
        frameCount++;
        if (timer.getElapsedTime() > sf::seconds(0.25)) {
            auto time = timer.getElapsedTime();
            frameTime = time.asMilliseconds() / frameCount;
            timer.restart();
            frameCount = 0;
        }
    }
};

class ClientEngine {
  public:
    // Window is pass in to init opengl
    // before running code here
    bool init(sf::Window& window);
    void runClient();

  private:
    void update();
    void render();

    void pollWindowEvents();

    ClientStateController m_controller;

    sf::Window* mp_window = nullptr;
    Keyboard m_keyboard;
    InputState m_inputState;

    ScriptEngine m_lua;
    ClientLuaCallbacks m_luaCallbacks;

    gui::GuiSystem m_gui;
    GuiRenderer m_guiRenderer;

    FPSCounter m_fpsCounter;

    // Rendering???
    gl::Framebuffer m_guiRenderTarget;
    gl::Framebuffer m_worldRenderTarget;

    gl::Shader m_screenShader;
    gl::VertexArray m_screenBuffer;

    Game m_game;
};
