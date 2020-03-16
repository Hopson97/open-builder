#include "client_engine.h"

#include "client.h"
#include "client_config.h"
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
#include <memory>
#include <thread>

#include <server_engine.h>

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

    class Game {
      public:
        bool initGame(ClientConfig config, const std::string& ipAddress);
        bool initGame(ClientConfig config);

        void stopGame();

        // Ran during game loop
        void onMouseRelease(sf::Mouse::Button button);
        void input(sf::Window& window, const Keyboard& keyboard,
                   const InputState& inputState);
        void update(float dt);
        void render();

      private:
        bool init(ClientConfig config);

        std::unique_ptr<Client> m_client;
        std::unique_ptr<ServerLauncher> m_serverLauncher;
    };

    //
    bool Game::initGame(ClientConfig config)
    {
        config.serverIp = LOCAL_HOST;

        m_serverLauncher =
            std::make_unique<ServerLauncher>(ServerConfig{8, 8}, sf::milliseconds(100));
        m_serverLauncher->runAsThread();

        return init(config);
    }

    bool Game::initGame(ClientConfig config, const std::string& ipAddress)
    {
        config.serverIp = ipAddress;

        return init(config);
    }

    bool Game::init(ClientConfig config)
    {
        m_client = std::make_unique<Client>();
        if (!m_client->init(config,
                            (float)config.windowWidth / (float)config.windowHeight)) {
            stopGame();
            return false;
        }
        return true;
    }

    void Game::stopGame()
    {
        if (m_serverLauncher) {
            m_serverLauncher->stop();
            m_serverLauncher.release();
        }
        if (m_client) {
            m_client->endGame();
            m_client->destroy();
            m_client.release();
        }
    }

    void Game::onMouseRelease(sf::Mouse::Button button)
    {
        if (m_client) {
            m_client->onMouseRelease(button);
        }
    }

    void Game::input(sf::Window& window, const Keyboard& keyboard,
                     const InputState& inputState)
    {
        if (m_client) {
            m_client->handleInput(window, keyboard, inputState);
        }
    }

    void Game::update(float dt)
    {
        if (m_client) {
            m_client->update(dt);
        }
    }

    void Game::render()
    {
        if (m_client) {
            m_client->render();
        }
    }
} // namespace

void runClientEngine(const ClientConfig& config)
{
    // Window/ OpenGL context setup
    sf::Window window;
    if (!createWindowInitOpengl(window, config)) {
        return; // EngineStatus::GLInitError;
    }

    // Client engine stuff
    ClientStateControl control;
    FPSCounter fps;
    sf::Clock gameTimer;

    // Input
    Keyboard keyboard;
    InputState inputState;

    // Init Lua scripting
    ScriptEngine scriptEngine;
    ClientLuaCallbacks callbacks;

    callbacks.initCallbacks(scriptEngine);
    luaInitGuiWidgetApi(scriptEngine);
    luaInitInputApi(scriptEngine, window, inputState);
    luaInitClientControlApi(scriptEngine, control);

    // Gui
    gui::GuiSystem gui(config.windowWidth, config.windowHeight, scriptEngine);

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
        gui.update();
        game.update(gameTimer.restart().asSeconds());

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
        gui.render();

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
        switch (control.currentState) {
            case ClientStateControl::StateId::CreateGame:
                if (game.initGame(config)) {
                    callbacks.onEnterGame();
                    control.currentState = ClientStateControl::StateId::InGame;
                }
                else {
                    callbacks.onError("Unable to create game.");
                    control.currentState = ClientStateControl::StateId::InMenu;
                }
                break;

            case ClientStateControl::StateId::JoinGame: {
                if (game.initGame(config, control.paramA)) {
                    callbacks.onEnterGame();
                    control.currentState = ClientStateControl::StateId::InGame;
                }
                else {
                    callbacks.onError("Unable to join game.");
                    control.currentState = ClientStateControl::StateId::InMenu;
                }
                break;
            } break;

            case ClientStateControl::StateId::LoadGame:
                if (game.initGame(config)) {
                    callbacks.onEnterGame();
                    control.currentState = ClientStateControl::StateId::InGame;
                }
                else {
                    callbacks.onError("Unable to load game.");
                    control.currentState = ClientStateControl::StateId::InMenu;
                }
                break;

            case ClientStateControl::StateId::ExitGame:
                game.stopGame();
                callbacks.onExitGame();
                control.currentState = ClientStateControl::StateId::InMenu;
                break;

            case ClientStateControl::StateId::Shutdown:
                isRunning = false;
                break;

            default:
                break;
        }
    }
    window.close();
}