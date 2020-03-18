#include <catch2/catch.hpp>

#include <client/client_controller.h>
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

using State = ClientStateControl::StateId;

void updateState(ClientStateControl& state)
{
    switch (state.currentState) {
        case State::CreateGame:
            state.currentState = State::InGame;
            break;

        case State::ExitGame:
            state.currentState = State::InMenu;
            break;

        default:
            break;
    }
}

TEST_CASE("The 'state' of the game can be safely controlled by the Lua")
{
    ScriptEngine engine;
    ClientStateControl state;

    luaInitClientControlApi(engine, state);

    SECTION("The engine blocks invalid state switching")
    {
        state.currentState = State::InMenu;
        engine.runLuaString("game.control.pause()");
        REQUIRE(state.currentState == State::InMenu);

        engine.runLuaString("game.control.exitGame()");
        updateState(state);
        REQUIRE(state.currentState == State::InMenu);

        state.currentState = State::InGame;
        engine.runLuaString("game.control.resume()");
        REQUIRE(state.currentState == State::InGame);

        engine.runLuaString("game.control.loadGame()");
        updateState(state);
        REQUIRE(state.currentState == State::InGame);
    }

    SECTION("Script is able to exit game from pause")
    {
        state.currentState = State::Paused;
        engine.runLuaString("game.control.exitGame()");
        updateState(state);
        REQUIRE(state.currentState == State::InMenu);
    }

    SECTION("Script is able to start game from menu")
    {
        state.currentState = State::InMenu;
        engine.runLuaString("game.control.createWorld(\"A\", \"B\")");
        updateState(state);
        REQUIRE(state.currentState == State::InGame);
    }
}