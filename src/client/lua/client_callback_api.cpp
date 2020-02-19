#include "client_lua_api.h"

#include "client_lua_callback.h"
#include <common/scripting/script_engine.h>

void initClientCallbackApi(ScriptEngine& engine, ClientLuaCallbacks& callback)
{
    engine.gameTable["onClientStartup"] = [&callback](sol::function f) {
        callback.onClientStartup.push_back(f);
    };
}