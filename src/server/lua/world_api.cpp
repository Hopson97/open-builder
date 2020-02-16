#include "server_lua_api.h"

#include <common/world/chunk.h>

#include <common/scripting/script_engine.h>

namespace {
void addApiChunk(ScriptEngine& script)
{
    auto chunkApi = script.lua.new_usertype<Chunk>("Chunk");
    chunkApi["setVoxel"] = [&](Chunk& chunk, int x, int y, int z, int voxel) {
        chunk.setVoxel({x, y, z}, voxel);
    };
}
} // namespace

void luaInitWorldApi(ScriptEngine& script)
{
    addApiChunk(script);
}