#include "client_lua_api.h"

#include "../engine_status.h"
#include <common/scripting/script_engine.h>

void luaInitClientControlApi(ScriptEngine& scriptEngine, ClientState& clientState)
{
    auto controlApi = scriptEngine.addTable("control");
    controlApi["isInGame"] = [&clientState] {
        return clientState == ClientState::InGame;
    };
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