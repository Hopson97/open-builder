#include "client_engine.h"

#include "client_config.h"
#include "client_state_controller.h"
#include "game.h"
#include "gl/framebuffer.h"
#include "gl/gl_errors.h"
#include "gl/primitive.h"
#include "gl/vertex_array.h"
#include "gui/gui_system.h"
#include "gui/widget/label_widget.h"
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

    //
} // namespace

void runClientEngine(const ClientConfig& config)
{
    // Window/ OpenGL context setup
    sf::Window window;
    if (!createWindowInitOpengl(window, config)) {
        return; // EngineStatus::GLInitError;
    }

    // Client engine stuff
    ClientStateController control;
    FPSCounter fps;
    sf::Clock gameTimer;

    // Input
    Keyboard keyboard;
    InputState inputState;

    // Init Lua scripting
    ScriptEngine scriptEngine;
    ClientLuaCallbacks callbacks;

    // Gui
    gui::GuiSystem gui(config.windowWidth, config.windowHeight);
    GuiRenderer guiRenderer;

    // Init all the lua api stuff
    callbacks.initCallbacks(scriptEngine);
    luaInitGuiWidgetApi(scriptEngine);
    luaInitInputApi(scriptEngine, window, inputState);
    luaInitClientControlApi(scriptEngine, control);
    luaInitGuiApi(scriptEngine, gui, &guiRenderer);

    // Run the lua file to init the client engine
    scriptEngine.runLuaFile("game/client/main.lua");
    callbacks.onClientStartup();

    Game game;

    //=============================================================

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

    bool isRunning = true;

    // Main loop of the client code
    while (isRunning) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (window.hasFocus()) {
                keyboard.update(event);
                gui.handleEvent(event);
            }
            switch (event.type) {
                case sf::Event::Closed:
                    isRunning = false;
                    break;

                case sf::Event::KeyReleased:
                    callbacks.onKeyboardKeyReleased(event.key.code);
                    break;

                case sf::Event::MouseButtonReleased:
                    game.onMouseRelease(event.mouseButton.button);
                    break;

                default:
                    break;
            }
        }

        // Input
        game.input(window, keyboard, inputState);

        // Update
        game.update(gameTimer.restart().asSeconds());
        gui.update();

        //=============================================================
        // Render
        //
        glEnable(GL_DEPTH_TEST);

        // World

        worldRenderTarget.bind();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        game.render();

        // GUI
        guiRenderTarget.bind();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        gui.render(guiRenderer);

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
        //=======================================================================

        // Stats
        fps.update();

        // TO DO Find some way to do this a lot more cleanly...
        // Switch to a different control if needed
        isRunning = control.executeAction(config, game, callbacks);
        switch (control.currentState) {
            case ClientStateController::StateId::JoinGame: {
                if (game.initGame(config, control.paramA)) {
                    callbacks.onEnterGame();
                    control.currentState = ClientStateController::StateId::InGame;
                }
                else {
                    callbacks.onError("Unable to join game.");
                    control.currentState = ClientStateController::StateId::InMenu;
                }
                break;
            } break;

            case ClientStateController::StateId::LoadGame:
                if (game.initGame(config)) {
                    callbacks.onEnterGame();
                    control.currentState = ClientStateController::StateId::InGame;
                }
                else {
                    callbacks.onError("Unable to load game.");
                    control.currentState = ClientStateController::StateId::InMenu;
                }
                break;

            case ClientStateController::StateId::ExitGame:
                game.stopGame();
                callbacks.onExitGame();
                control.currentState = ClientStateController::StateId::InMenu;
                break;

            case ClientStateController::StateId::Shutdown:
                isRunning = false;
                break;

            default:
                break;
        }
    }
    window.close();
}