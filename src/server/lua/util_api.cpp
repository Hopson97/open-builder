#include "server_lua_api.h"
#include <common/lua/script_engine.h>
#include <common/util/random_number_generator.h>

namespace {
    void addRandomNumberGeneratorApi(ScriptEngine& scriptEngine)
    {
        auto rngApi =
            scriptEngine.lua.new_usertype<RandomNumberGenerator>("RandomNumberGenerator");
        rngApi["nextInt"] = &RandomNumberGenerator::nextInt;
        rngApi["nextFloat"] = &RandomNumberGenerator::nextFloat;
    }
} // namespace

void luaInitUtilApi(ScriptEngine& scriptEngine)
{
    addRandomNumberGeneratorApi(scriptEngine);
}