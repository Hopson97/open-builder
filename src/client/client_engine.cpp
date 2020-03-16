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

    /**
     * @brief The 'F3' prompt showing info like # of chunks
     * drawn, frame time, etc
     */
    struct DebugGui {
        gui::OverlayDefinition def;
        gui::Overlay overlay;

        gui::LabelWidget& label;

        DebugGui()
            : overlay(def)
            , label(*overlay.addLabel())
        {
            label.setPosition({0, 5, 0, GUI_HEIGHT - 25});
            label.setTextSize(32);
            label.setText("TESTING");
        }
    };

    //  ===================================================
    //
    //            T E M P O R A R Y   S T A R T
    //
    // ====================================================

    struct Game {
        virtual ~Game() = default;
        virtual int startGame(ClientConfig config, float windowAspect) = 0;
        virtual void endGame() = 0;

        std::unique_ptr<Client> client;
    };

    struct LocalGame : public Game {
        std::unique_ptr<ServerLauncher> serverLauncher;

        ~LocalGame()
        {
            endGame();
        }

        int startGame(ClientConfig config, float windowAspect) final override
        {
            config.serverIp = LOCAL_HOST;
            serverLauncher = std::make_unique<ServerLauncher>(ServerConfig{4, 5},
                                                              sf::milliseconds(500));
            serverLauncher->runAsThread();

            client = std::make_unique<Client>();
            if (!client->init(config, windowAspect)) {
                endGame();
                return -1;
            }
            return 0;
        }

        void endGame() final override
        {
            if (serverLauncher) {
                serverLauncher->stop();
                serverLauncher.release();
            }
            if (client) {
                client->endGame();
                client->destroy();
                client.release();
            }
        }
    };

    struct RemoteGame : public Game {
        RemoteGame(const std::string& serverIp)
            : serverIpAddress(serverIp)
        {
        }

        ~RemoteGame()
        {
            endGame();
        }

        int startGame(ClientConfig config, float windowAspect) final override
        {
            config.serverIp = serverIpAddress;
            client = std::make_unique<Client>();
            if (!client->init(config, windowAspect)) {
                endGame();
                return -1;
            }
            return 0;
        }

        void endGame() final override
        {
            if (client) {
                client->endGame();
                client->destroy();
                client.release();
            }
        }

        const std::string serverIpAddress;
    };

    //  ===================================================
    //
    //              T E M P O R A R Y    E N D
    //
    // ====================================================
} // namespace

class ClientEngine {
  public:
    ClientEngine();
    void run();

  private:
    sf::Window m_window;
};

ClientEngine::ClientEngine()
{
}

void ClientEngine::run()
{
}

void runClientEngine(const ClientConfig& config)
{
    // ClientEngine engine;
    // engine.run();
    // return;

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

    // Init the "debug prompt" F3 GUI
    DebugGui debugGui;

    // Init Lua scripting
    ScriptEngine scriptEngine;
    ClientLuaCallbacks callbacks(scriptEngine);
    luaInitGuiWidgetApi(scriptEngine);
    luaInitInputApi(scriptEngine, window, inputState);
    luaInitClientControlApi(scriptEngine, control);

    // Gui
    gui::GuiSystem gui(config.windowWidth, config.windowHeight, scriptEngine);

    // Run the lua file to init the client engine
    scriptEngine.runLuaFile("game/client/main.lua");
    callbacks.onClientStartup();

    // Client client;
    //============================================================
    //              Tempory approach!
    std::unique_ptr<Game> game;

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
                    if (game && game->client) {

                        game->client->onMouseRelease(event.mouseButton.button,
                                                     event.mouseButton.x,
                                                     event.mouseButton.y);
                    }

                default:
                    break;
            }
        }

        // Input
        if (game && game->client) {
            game->client->handleInput(window, keyboard, inputState);
        }

        // Update
        gui.update();
        if (game && game->client) {

            game->client->update(gameTimer.restart().asSeconds(), fps.frameTime);
        }

        //=============================================================
        // Render
        //
        glEnable(GL_DEPTH_TEST);

        // World
        if (game && game->client) {

            worldRenderTarget.bind();
            glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
            game->client->render(debugGui.label);
        }

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

        auto startGame = [&]() {
            auto result = game->startGame(config, getWindowAspect(window));
            if (result == -1) {
                callbacks.onError("Unable to connect to server.");
                control.currentState = ClientStateControl::StateId::InMenu;
                game.release();
            }
            else {
                callbacks.onEnterGame();
                control.currentState = ClientStateControl::StateId::InGame;
            }
        };

        // TO DO Find some way to do this a lot more cleanly...
        // Switch to a different control if needed
        switch (control.currentState) {
            case ClientStateControl::StateId::CreateGame: {
                game = std::make_unique<LocalGame>();
                startGame();
            } break;

            case ClientStateControl::StateId::JoinGame: {
                game = std::make_unique<RemoteGame>(control.paramA);
                startGame();
            } break;

            case ClientStateControl::StateId::LoadGame: {
                game = std::make_unique<LocalGame>();
                startGame();
            } break;

            case ClientStateControl::StateId::ExitGame:
                game->endGame();
                worldRenderTarget.bind();
                glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
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
    return;
}
