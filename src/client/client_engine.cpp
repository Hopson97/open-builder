#include "client_engine.h"

#include "client.h"
#include "gl/gl_errors.h"
#include "gui/gui_master.h"
#include "lua/client_lua_api.h"
#include "lua/client_lua_callback.h"
#include "renderer/chunk_renderer.h"
#include "window.h"
#include <SFML/System/Clock.hpp>
#include <common/scripting/script_engine.h>
#include <glad/glad.h>
namespace {
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

bool initOpenGL(const sf::Window& window)
{
    if (!gladLoadGL()) {
        return false;
    }
#ifndef __APPLE__
    initGLDebug();
#endif
    glCheck(glClearColor(0.25f, 0.75f, 1.0f, 1.0f));
    glCheck(glViewport(0, 0, window.getSize().x, window.getSize().y));
    glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glCheck(glEnable(GL_DEPTH_TEST));
    return true;
}
} // namespace

EngineStatus runClientEngine(const ClientConfig& config)
{
    // Window/ OpenGL context setup
    sf::Window window;
    window.setFramerateLimit(60);
    createWindow(window, config);
    if (!initOpenGL(window)) {
        return EngineStatus::GLInitError;
    }

    // Client engine stuff
    EngineStatus status = EngineStatus::Ok;
    Keyboard keys;
    FPSCounter fps;
    GuiMaster guiMaster(window.getSize().x, window.getSize().y);
    sf::Clock gameTimer;
    int tickCount = 0;

    // Init the "debug prompt" F3 GUI
    auto container = guiMaster.addGui();
    auto debugStatsText = container->addText();
    debugStatsText->setFontSize(16);
    debugStatsText->setPosition({0.0f, 4.0f, 1.0f, -16.0f});

    // Init Lua scripting
    ScriptEngine scriptEngine;
    ClientLuaCallbacks callbacks;

    initClientCallbackApi(scriptEngine, callbacks);
    initGuiApi(scriptEngine, guiMaster);
    scriptEngine.runLuaFile("game/client/main.lua");

    callbacks.runClientStartupCallbacks();

    // Init screens here
    Client client;
    if (!client.init(config, getWindowAspect(window))) {
        return EngineStatus::CouldNotConnect;
    }

    // Main loop of the client code
    while (status == EngineStatus::Ok) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (window.hasFocus()) {
                keys.update(event);
            }
            switch (event.type) {
                case sf::Event::Closed:
                    status = EngineStatus::Exit;
                    break;

                case sf::Event::KeyReleased:
                    client.onKeyRelease(event.key.code);
                    switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            status = EngineStatus::Exit;
                            break;

                        default:
                            break;
                    }
                    break;

                case sf::Event::MouseButtonReleased:
                    client.onMouseRelease(event.mouseButton.button, event.mouseButton.x,
                                          event.mouseButton.y);
                    break;

                default:
                    break;
            }
        }
        tickCount++;

        // Input
        client.handleInput(window, keys);

        // Update
        client.update(gameTimer.restart().asSeconds(), fps.frameTime);

        // Render
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        client.render(*debugStatsText);

        guiMaster.render();
        window.display();

        // Stats
        fps.update();
    }
    client.endGame();
    return status;
}
