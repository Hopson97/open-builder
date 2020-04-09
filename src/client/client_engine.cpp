#include "client_engine.h"

#include "client_config.h"
#include "gl/primitive.h"
#include "lua/client_lua_api.h"
#include "window.h"

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
    return true;
}

void ClientEngine::runClient()
{
    while (mp_window->isOpen()) {
        pollWindowEvents();
        m_game.handleInput(m_keyboard, m_inputState);
        update();
        render();

        if (!m_controller.executeAction(m_game, m_luaCallbacks)) {
            mp_window->close();
        }
    }
}

void ClientEngine::update()
{
    static sf::Clock clock;
    m_fpsCounter.update();
    m_game.tick(clock.restart().asSeconds());
    m_gui.update();
    if (((int)m_fpsCounter.frameCount % 256) == 0) {
        std::cout << m_fpsCounter.frameTime << '\n';
    }
}

void ClientEngine::render()
{
    glEnable(GL_DEPTH_TEST);

    // World
    m_worldRenderTarget.bind();
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    m_game.render();

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
            m_game.handleEvent(event);
        }
        switch (event.type) {
            case sf::Event::MouseWheelScrolled:
                m_luaCallbacks.onMouseWheelScroll(event.mouseWheelScroll);
                break;

            case sf::Event::KeyReleased:
                m_luaCallbacks.onKeyboardKeyReleased(event.key.code);
                break;

            default:
                break;
        }
    }
}
