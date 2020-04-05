#pragma once

#include "../lua/server_lua_callback.h"
#include <common/lua/script_engine.h>
#include <common/network/packet.h>
#include <common/world/biome.h>
#include <common/world/chunk_manager.h>
#include <common/world/entity_state.h>
#include <common/world/voxel_data.h>

class ServerWorld {
  public:
    ServerWorld();

    void tick();

    const std::vector<EntityState>& getEntities() const;
    void serialiseEntities(ServerPacket& packet) const;

    u32 addEntity();
    void removeEntity(u32 id);
    EntityState& findEntity(u32 id);

    const VoxelDataManager& getVoxelData() const;
    const BiomeDataManager& getBiomeData() const;

  private:
    std::vector<EntityState> m_entities;
    u32 entityCount = 0;
    ChunkManager m_chunks;

    ScriptEngine m_lua;
    ServerLuaCallbacks m_luaCallbacks;

    VoxelDataManager m_voxelData;
    BiomeDataManager m_biomeData;
};