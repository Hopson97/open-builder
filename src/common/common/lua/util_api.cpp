#include "script_engine.h"

#include "../util/random_number_generator.h"

namespace {
    void addRandomNumberGeneratorApi(ScriptEngine& scriptEngine)
    {
        auto rngApi =
            scriptEngine.lua.new_usertype<RandomNumberGenerator>("RandomNumberGenerator");
        rngApi["nextInt"] = &RandomNumberGenerator::nextInt;
        rngApi["nextFloat"] = &RandomNumberGenerator::nextFloat;
    }
} // namespace

void ScriptEngine::luaInitUtilApi()
{
    addRandomNumberGeneratorApi(*this);
}