#include "client_engine.h"

#include "client.h"
#include "client_config.h"
#include "gl/framebuffer.h"
#include "gl/gl_errors.h"
#include "gl/primitive.h"
#include "gl/vertex_array.h"
#include "gui/gui_system.h"
#include "input/input_state.h"
#include "lua/client_lua_api.h"
#include "lua/client_lua_callback.h"
#include "renderer/chunk_renderer.h"
#include "renderer/gui_renderer.h"
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
        glCheck(glClearColor(0.25f, 0.75f, 1.0f, 0.0f));
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
    createWindow(window, config);
    if (!initOpenGL(window)) {
        return EngineStatus::GLInitError;
    }

    // Client engine stuff
    ClientEngineState state;
    FPSCounter fps;
    sf::Clock gameTimer;

    // Input
    Keyboard keys;
    InputState inputState;

    // Init the "debug prompt" F3 GUI
    // DebugGui debugGui;

    // Init Lua scripting
    ScriptEngine scriptEngine;
    ClientLuaCallbacks callbacks(scriptEngine);
    luaInitGuiWidgetApi(scriptEngine);
    luaInitInputApi(scriptEngine, window, inputState);
    luaInitClientControlApi(scriptEngine, state.stage);

    // Gui
    gui::GuiSystem gui(config.windowWidth, config.windowHeight, scriptEngine);

    // Run the lua file to init the client engine
    scriptEngine.runLuaFile("game/client/main.lua");
    callbacks.onClientStartup();

    // Client client;
    // if (!client.init(config, getWindowAspect(window))) {
    //    return EngineStatus::CouldNotConnect;
    //}

    // Temp render stuff for testing
    gl::Framebuffer guiRenderTarget;
    gl::Framebuffer worldRenderTarget;
    gl::Shader screenShader;
    gl::VertexArray screenVAO = makeScreenQuadVertexArray();

    int width = config.windowWidth;
    int height = config.windowHeight;
    guiRenderTarget.create(static_cast<unsigned>(GUI_WIDTH),
                           static_cast<unsigned>(GUI_HEIGHT));
    worldRenderTarget.create(width, height);
    screenShader.create("minimal", "minimal");

    // Main loop of the client code
    while (state.status == EngineStatus::Ok) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (window.hasFocus()) {
                keys.update(event);
                gui.handleEvent(event);
            }
            switch (event.type) {
                case sf::Event::Closed:
                    state.status = EngineStatus::Exit;
                    break;

                case sf::Event::KeyReleased:
                    callbacks.onKeyboardKeyReleased(event.key.code);
                    // client.onKeyRelease(event.key.code);
                    break;

                case sf::Event::MouseButtonReleased:
                    // client.onMouseRelease(event.mouseButton.button,
                    // event.mouseButton.x,
                    //                      event.mouseButton.y);
                    break;

                default:
                    break;
            }
        }

        // Input
        // client.handleInput(window, keys, inputState);

        // Update
        gui.update();
        // client.update(gameTimer.restart().asSeconds(), fps.frameTime);

        //
        // Render
        //
        glEnable(GL_DEPTH_TEST);

        // World
        worldRenderTarget.bind();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        // client.render(debugGui.label);

        // GUI
        guiRenderTarget.bind();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        gui.render();
        // guiRenderer.render(debugGui.overlay);

        // Buffer to window
        gl::unbindFramebuffers(width, height);
        glDisable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT);
        auto drawable = screenVAO.getDrawable();
        drawable.bind();
        screenShader.bind();

        worldRenderTarget.bindTexture();
        drawable.draw();

        glEnable(GL_BLEND);

        guiRenderTarget.bindTexture();
        drawable.draw();

        window.display();
        glDisable(GL_BLEND);

        // Stats
        fps.update();
        switch (state.stage) {
            case ClientState::StartGame:
                state.stage = ClientState::InGame;
                break;

            case ClientState::ExitGame:
                state.stage = ClientState::InMenu;
                break;

            case ClientState::Shutdown:
                state.status = EngineStatus::Exit;
                break;

            default:
                break;
        }
    }
    // client.endGame();
    return state.status;
}
