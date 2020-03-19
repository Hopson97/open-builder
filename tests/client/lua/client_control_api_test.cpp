#include <catch2/catch.hpp>

#include <client/client_state_controller.h>
#include <client/lua/client_lua_api.h>
#include <client/lua/client_lua_callback.h>
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

using State = ClientStateController::StateId;

TEST_CASE("The 'state' of the game can be safely controlled by the Lua")
{
    ScriptEngine engine;
    ClientStateController controller;
    ClientLuaCallbacks callbacks;
    callbacks.initCallbacks(engine);

    luaInitClientControlApi(engine, controller);
    /*
    SECTION("The engine blocks invalid state switching")
    {
        controller.currentState = State::InMenu;
        engine.runLuaString("game.control.pause()");
        REQUIRE(state.currentState == State::InMenu);

        engine.runLuaString("game.control.exitGame()");
        controller
        REQUIRE(state.currentState == State::InMenu);

        state.currentState = State::InGame;
        engine.runLuaString("game.control.resume()");
        REQUIRE(state.currentState == State::InGame);

        engine.runLuaString("game.control.loadWorld(\"Test\")");
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
    */
}