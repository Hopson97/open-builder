#include "client_lua_api.h"

#include "../client_state_controller.h"
#include <common/lua/script_engine.h>

using State = ClientStateController::StateId;

void luaInitClientControlApi(ScriptEngine& scriptEngine,
                             ClientStateController& clientState)
{
    auto controlApi =
        scriptEngine.lua.new_usertype<ClientStateController>("ClientStateController");

    scriptEngine.gameTable["control"] = [&clientState]() -> ClientStateController& {
        return clientState;
    };

    controlApi["createWorld"] = &ClientStateController::createWorld;
    controlApi["loadWorld"] = &ClientStateController::loadWorld;
    controlApi["joinGame"] = &ClientStateController::joinWorld;
    controlApi["isInGame"] = [](const ClientStateController& controller) {
        return controller.currentState() == State::InGame;
    };
    controlApi["pause"] = &ClientStateController::pauseGame;
    controlApi["resume"] = &ClientStateController::resumeGame;
    controlApi["exitGame"] = &ClientStateController::exitGame;
    controlApi["shutdown"] = &ClientStateController::shutdown;
}
