#include "client_engine.h"

#include "client.h"
#include "gl/gl_errors.h"
#include "gui/overlay.h"
#include "lua/client_lua_api.h"
#include "lua/client_lua_callback.h"
#include "renderer/chunk_renderer.h"
#include "renderer/gui_renderer.h"
#include "window.h"
#include <SFML/System/Clock.hpp>
#include <common/scripting/script_engine.h>
#include <glad/glad.h>

#include "gl/framebuffer.h"

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
    sf::Clock gameTimer;
    int tickCount = 0;

    // Init the "debug prompt" F3 GUI
    /*
    auto container = guiMaster.addGui();
    auto debugStatsText = container->addText();
    debugStatsText->setFontSize(16);
    debugStatsText->setPosition({0.0f, 4.0f, 1.0f, -16.0f});
    */

    // Init Lua scripting
    ScriptEngine scriptEngine;
    ClientLuaCallbacks callbacks(scriptEngine);

    // Gui
    gui::OverlayFactory overlayFactory;
    gui::OverlayStack overlayStack;
    GuiRenderer guiRenderer(window.getSize().x, window.getSize().y);

    // Lua API set up
    luaInitGuiApi(scriptEngine, overlayFactory, overlayStack, &guiRenderer);
    luaInitGuiWidgetApi(scriptEngine);

    // overlayStack.pushLayer(overlayFactory.createOverlay("main_menu"));

    scriptEngine.runLuaFile("game/client/main.lua");
    callbacks.onClientStartup();

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
                    overlayStack.handleKeyRelease(event.key.code);
                    switch (event.key.code) {
                        case sf::Keyboard::Escape:
                            status = EngineStatus::Exit;
                            break;

                        default:
                            break;
                    }
                    break;

                case sf::Event::MouseMoved:
                    overlayStack.handleMouseMove(event.mouseMove);
                    break;

                case sf::Event::MouseButtonReleased:
                    overlayStack.handleClick(event.mouseButton.button,
                                             event.mouseButton.x, event.mouseButton.y);
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
        overlayStack.update();
        client.update(gameTimer.restart().asSeconds(), fps.frameTime);

        // Render
        gl::unbindFramebuffers(window.getSize().x, window.getSize().y);
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        client.render();

        for (auto& overlay : overlayStack.overlays) {
            overlay->prepareWidgetsForRender();
            guiRenderer.render(*overlay);
        }
        window.display();

        // Stats
        fps.update();
    }
    client.endGame();
    return status;
}
