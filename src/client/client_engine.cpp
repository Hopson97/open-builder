#include "client_engine.h"

#include "lua/client_lua_api.h"
#include "gl/primitive.h"
#include "window.h"

#include "state/main_menu_state.h"

bool ClientEngine::init(sf::Window& window)
{
    mp_window = &window;

    // Init all the lua api stuff
    m_luaCallbacks.initCallbacks(m_lua);
    luaInitGuiWidgetApi(m_lua);
    luaInitInputApi(m_lua, window, m_inputState);
    luaInitClientControlApi(m_lua, m_controller);
    luaInitGuiApi(m_lua, m_gui, &m_guiRenderer);

    m_lua.runLuaFile("game/client/main.lua");
    m_luaCallbacks.onClientStartup();

    m_guiRenderTarget.create(GUI_WIDTH, GUI_HEIGHT);
    m_worldRenderTarget.create(ClientConfig::get().windowHeight,
                               ClientConfig::get().windowWidth);

    m_screenShader.create("minimal", "minimal");
    m_screenBuffer = makeScreenQuadVertexArray();

    m_stateManager.push(m_gui, std::make_unique<MainMenuGameState>(m_stateManager));
    m_stateManager.updateStack(m_gui);

    return true;
}

void ClientEngine::runClient()
{
    while (mp_window->isOpen() && !m_stateManager.isEmpty()) {
        pollWindowEvents();

        m_stateManager.handleInput();
        m_game.input(*mp_window, m_keyboard, m_inputState);

        update();
        render();

        m_stateManager.updateStack(m_gui);
        if (!m_controller.executeAction(m_game, m_luaCallbacks)) {
            mp_window->close();
        }
    }
}

void ClientEngine::update()
{
    m_stateManager.handleUpdate();
    m_fpsCounter.update();
    m_game.update(0.16f);
    m_gui.update();
}

void ClientEngine::render()
{
    glEnable(GL_DEPTH_TEST);

    // World
    m_worldRenderTarget.bind();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    m_game.render();

    m_stateManager.handleRender();

    // GUI
    m_guiRenderTarget.bind();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    m_gui.render(m_guiRenderer);

    // Buffer to window
    gl::unbindFramebuffers(ClientConfig::get().windowWidth,
                           ClientConfig::get().windowHeight);
    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT);
    auto drawable = m_screenBuffer.getDrawable();
    drawable.bind();
    m_screenShader.bind();

    m_worldRenderTarget.bindTexture();
    drawable.draw();
    glEnable(GL_BLEND);

    m_guiRenderTarget.bindTexture();
    drawable.draw();

    mp_window->display();
    glDisable(GL_BLEND);
}

void ClientEngine::pollWindowEvents()
{
    sf::Event event;
    while (mp_window->pollEvent(event)) {
        if (mp_window->hasFocus()) {
            m_keyboard.update(event);
            m_gui.handleEvent(event);
        }
        switch (event.type) {
            case sf::Event::MouseWheelScrolled:
                m_luaCallbacks.onMouseWheelScroll(event.mouseWheelScroll);
                break;

            case sf::Event::KeyReleased:
                m_luaCallbacks.onKeyboardKeyReleased(event.key.code);
                break;

            case sf::Event::MouseButtonReleased:
                m_game.onMouseRelease(event.mouseButton.button);
                break;

            default:
                break;
        }
    }
}
