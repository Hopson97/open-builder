#include "client_engine.h"

#include "gl/gl_errors.h"
#include "gui/gui_master.h"
#include "lua/client_lua_api.h"
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
} // namespace

EngineStatus runClientEngine(const ClientConfig& config)
{
    (void)config;
    return EngineStatus::Ok;
}

EngineStatus runClientEngine2(const ClientConfig& config)
{
    // Window setup
    sf::Window window;
    window.setFramerateLimit(60);
    createWindow(window, config);

    // OpenGL setup
    if (!gladLoadGL()) {
        return EngineStatus::GLInitError;
    }
#ifndef __APPLE__
    initGLDebug();
#endif
    glCheck(glClearColor(0.25f, 0.75f, 1.0f, 1.0f));
    glCheck(glViewport(0, 0, window.getSize().x, window.getSize().y));
    glCheck(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    glCheck(glEnable(GL_DEPTH_TEST));

    // Client engine stuff
    EngineStatus status = EngineStatus::Ok;
    Keyboard keys;
    FPSCounter fps;
    GuiMaster guiMaster(window.getSize().x, window.getSize().y);
    int tickCount = 0;

    // Init Lua scripting
    ScriptEngine scriptEngine;
    initGuiApi(scriptEngine, guiMaster);
    scriptEngine.runLuaFile("game/client/main.lua");

    // Init screens here

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

                default:
                    break;
            }
        }
        tickCount++;

        // Input

        // Update

        // Render
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        // 3d stuff here

        guiMaster.render();
        window.display();
    }
    return status;
}
