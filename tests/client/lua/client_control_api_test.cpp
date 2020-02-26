#include <catch2/catch.hpp>

#include <client/engine_status.h>
#include <client/lua/client_lua_api.h>
#include <common/scripting/script_engine.h>

const std::string guiCreateScript = R"(

    function create(overlay, data)

    end

    game.gui.addGui{
        id = "test_gui",
        title = "Test GUI",
        create = create
    }
)";

void updateState(ClientState& state)
{
    switch (state) {
        case ClientState::StartGame:
            state = ClientState::InGame;
            break;

        case ClientState::ExitGame:
            state = ClientState::InMenu;
            break;

        default:
            break;
    }
}

TEST_CASE("The 'state' of the game can be safely controlled by the Lua")
{
    ScriptEngine engine;
    ClientState state = ClientState::InMenu;

    luaInitClientControlApi(engine, state);

    SECTION("The engine blocks invalid state switching")
    {
        state = ClientState::InMenu;
        engine.runLuaString("game.control.pause()");
        REQUIRE(state == ClientState::InMenu);

        engine.runLuaString("game.control.exitGame()");
        updateState(state);
        REQUIRE(state == ClientState::InMenu);

        state = ClientState::InGame;
        engine.runLuaString("game.control.resume()");
        REQUIRE(state == ClientState::InGame);

        engine.runLuaString("game.control.startGame()");
        updateState(state);
        REQUIRE(state == ClientState::InGame);
    }

    SECTION("Script is able to exit game from pause")
    {
        state = ClientState::Paused;
        engine.runLuaString("game.control.exitGame()");
        updateState(state);
        REQUIRE(state == ClientState::InMenu);
    }

    SECTION("Script is able to start game from menu")
    {
        state = ClientState::InMenu;
        engine.runLuaString("game.control.startGame()");
        updateState(state);
        REQUIRE(state == ClientState::InGame);
    }
}