#include "client_lua_api.h"

#include "../client_state_controller.h"
#include <common/scripting/script_engine.h>

using State = ClientStateController::StateId;

void luaInitClientControlApi(ScriptEngine& scriptEngine,
                             ClientStateController& clientState)
{
    auto controlApi = scriptEngine.addTable("control");

    controlApi["pause"] = [&clientState] { clientState.pauseGame(); };
    controlApi["resume"] = [&clientState] { clientState.resumeGame(); };

    controlApi["exitGame"] = [&clientState] { clientState.exitGame(); };
    controlApi["shutdown"] = [&clientState] { clientState.shutdown(); };

    controlApi["createWorld"] = [&clientState](const std::string& name,
                                               const std::string& seed) {
        clientState.createWorld(name, seed);
    };

    controlApi["loadWorld"] = [&clientState](const std::string& name) {
        clientState.loadWorld(name);
    };

    controlApi["joinGame"] = [&clientState](const std::string& ip) {
        clientState.joinWorld(ip);
    };

    controlApi["isInGame"] = [&clientState] {
        return clientState.currentState() == State::InGame;
    };
}
