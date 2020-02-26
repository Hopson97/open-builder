#include "client_lua_api.h"

#include "../engine_status.h"
#include <common/scripting/script_engine.h>

void luaInitClientControlApi(ScriptEngine& scriptEngine, ClientState& clientState)
{
    auto stateEnum = scriptEngine.addTable("State");
    stateEnum["InMenu"] = ClientState::InMenu;
    stateEnum["InGame"] = ClientState::InGame;
    stateEnum["Paused"] = ClientState::Paused;

    stateEnum["ExitGame"] = ClientState::ExitGame;
    stateEnum["StartGame"] = ClientState::StartGame;
    stateEnum["Shutdown"] = ClientState::Shutdown;

    auto controlApi = scriptEngine.addTable("control");
    controlApi["currentState"] = [&clientState] { return clientState; };
    controlApi["shutdown"] = [&clientState] { clientState = ClientState::Shutdown; };

    controlApi["pause"] = [&clientState] {
        if (clientState == ClientState::InGame) {
            clientState = ClientState::Paused;
        }
    };
    controlApi["resume"] = [&clientState] {
        if (clientState == ClientState::Paused) {
            clientState = ClientState::InGame;
        }
    };
    controlApi["exitGame"] = [&clientState] {
        if (clientState == ClientState::Paused) {
            clientState = ClientState::ExitGame;
        }
    };

    controlApi["startGame"] = [&clientState] {
        if (clientState == ClientState::InMenu) {
            clientState = ClientState::StartGame;
        }
    };
}