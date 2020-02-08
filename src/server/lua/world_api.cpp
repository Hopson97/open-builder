#include "lua_api.h"

#include <common/world/chunk.h>

#include <common/scripting/script_engine.h>

namespace {
void addApiChunk(ScriptEngine& script)
{
    auto chunkApi = script.lua.new_usertype<Chunk>("Chunk");
    chunkApi["setBlock"] = [&](Chunk& chunk, int x, int y, int z, int block) {
        chunk.setBlock({x, y, z}, block);
    };
}
} // namespace

void luaInitWorldApi(ScriptEngine& script)
{
    addApiChunk(script);
}