#include "server_lua_api.h"
#include <common/scripting/script_engine.h>
#include <common/util/random_number_generator.h>

namespace {
void addRandomNumberGeneratorApi(ScriptEngine& engine)
{
    auto rngApi = engine.lua.new_usertype<RandomNumberGenerator>("RandomNumberGenerator");
    rngApi["nextInt"] = &RandomNumberGenerator::nextInt;
    rngApi["nextFloat"] = &RandomNumberGenerator::nextFloat;
}
} // namespace

void luaInitUtilApi(ScriptEngine& engine)
{
    addRandomNumberGeneratorApi(engine);
}