#include <catch2/catch.hpp>

#include <client/client_state_controller.h>
#include <client/game/game.h>
#include <client/lua/client_lua_api.h>
#include <client/lua/client_lua_callback.h>
#include <common/lua/script_engine.h>

using State = ClientStateController::StateId;

TEST_CASE("The 'state' of the game can be safely controlled by the Lua")
{
    ScriptEngine engine;
    ClientStateController controller;
    ClientLuaCallbacks callbacks;
    callbacks.initCallbacks(engine);
    Game game;

    luaInitClientControlApi(engine, controller);

    SECTION("The game can be shutdown via the lua API")
    {
        engine.runLuaString("game.control():shutdown()");
        REQUIRE(controller.executeAction(game, callbacks) == false);
    }

    // @TODO After the refactor, the rest of these tests will be able to be written
    // Right now they cannot, as they use opengl functions etc etc etc

    // SECTION("A world can be created via the lua API")
    //{
    //    engine.runLuaString("game.control.createWorld(\"a\", \"b\")");
    //    controller.executeAction({}, game, callbacks);
    //    REQUIRE(controller.currentState() == State::InGame);
    //}
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
