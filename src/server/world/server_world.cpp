#include "server_world.h"

#include <cassert>
#include "../lua/server_lua_api.h"

ServerWorld::ServerWorld()
    : m_luaCallbacks(m_lua)
{
    m_entities.resize(1024);
    
    // Initialise the Lua API
    luaInitDataApi(m_lua, m_biomeData, m_voxelData);
    luaInitWorldApi(m_lua);

    // Load game data. Must be in this order!
    m_lua.lua["path"] = "game/";
    m_lua.runLuaFile("game/voxels.lua");
    m_lua.runLuaFile("game/biomes.lua");
}

void ServerWorld::tick()
{
    /*
    for (int i = 1; i < m_entities.size(); i++) {
        EntityState& state = m_entities[i];
        if (state.active) {
        }
    }
    */
}

const std::vector<EntityState>& ServerWorld::getEntities() const
{
    return m_entities;
}

void ServerWorld::serialiseEntities(ServerPacket& packet) const
{
    packet.write(entityCount);
    for (int i = 1; i < m_entities.size(); i++) {
        const auto& state = m_entities[i];
        if (state.active) {
            packet.write(static_cast<u32>(i));
            packet.write(state.position);
            packet.write(state.rotation);
        }
    }
}

u32 ServerWorld::addEntity()
{
    for (int i = 1; i < m_entities.size(); i++) {
        auto& state = m_entities[i];
        if (!state.active) {
            entityCount++;
            state.active = true;
            return i;
        }
    }
    return 0;
}

void ServerWorld::removeEntity(u32 id)
{
    assert(id < m_entities.size());
    m_entities[id].active = false;
    entityCount--;
}

EntityState& ServerWorld::findEntity(u32 id)
{
    assert(id < m_entities.size());
    return m_entities[id];
}

const VoxelDataManager& ServerWorld::getVoxelData() const
{
    return m_voxelData;
}

const BiomeDataManager& ServerWorld::getBiomeData() const
{
    return m_biomeData;
}
