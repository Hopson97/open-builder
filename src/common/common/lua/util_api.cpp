#include "script_engine.h"

#include "../util//random_number_generator.h"
#include "../util/string_data_mapper.h"

namespace {
    void addRandomNumberGeneratorApi(ScriptEngine& scriptEngine)
    {
        auto rngApi =
            scriptEngine.lua.new_usertype<RandomNumberGenerator>("RandomNumberGenerator");
        rngApi["nextInt"] = &RandomNumberGenerator::nextInt;
        rngApi["nextFloat"] = &RandomNumberGenerator::nextFloat;
    }

    void addStringDataMapperApi(ScriptEngine& scriptEngine)
    {
        auto dataMapperApi =
            scriptEngine.lua.new_usertype<StringDataMapper>("StringDataMapper");
        dataMapperApi["getString"] = &StringDataMapper::getString;
        dataMapperApi["getInt"] = &StringDataMapper::getInt;
        dataMapperApi["getFloat"] = &StringDataMapper::getFloat;
    }
} // namespace

void ScriptEngine::luaInitUtilApi()
{
    addRandomNumberGeneratorApi(*this);
    addStringDataMapperApi(*this);
}