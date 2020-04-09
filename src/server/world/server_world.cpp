#include "server_world.h"

#include "../lua/server_lua_api.h"
#include <cassert>

#include "terrain_generation.h"
#include <common/maths.h>

ServerWorld::ServerWorld(int size)
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

    m_voxelData.initCommonVoxelTypes();

    for (int z = 0; z < size; z++) {
        for (int x = 0; x < size; x++) {
            auto chunks =
                generateTerrain(m_chunks, x, z, m_voxelData, m_biomeData, 9000, size);
            for (auto& chunk : chunks)
                m_currentChunks.insert(chunk);
        }
    }
}

void ServerWorld::tick()
{
    /*
    while (!m_chunkGenerationQueue.empty()) {
        auto pos = m_chunkGenerationQueue.front();
        m_chunkGenerationQueue.pop();
        std::cout << "Generating terrain\n";
        generateTerrain(m_chunks, pos.x, pos.z, m_voxelData, m_biomeData, 1234, 16);
        m_currentChunks.insert(pos);
    }
    */
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
    for (u32 i = 1; i < m_entities.size(); i++) {
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
    for (u32 i = 1; i < m_entities.size(); i++) {
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

const Chunk* ServerWorld::getChunk(const ChunkPosition& chunkPosition)
{
    if (m_currentChunks.find(chunkPosition) != m_currentChunks.end()) {
        return &m_chunks.getChunk(chunkPosition);
    }
    else {
        m_chunkGenerationQueue.push(chunkPosition);
        return nullptr;
    }
}

const ChunkPositionMap<Chunk>& ServerWorld::getChunks() const
{
    return m_chunks.chunks();
}

glm::vec3 ServerWorld::getPlayerSpawnPosition(u32 playerId)
{
    int x = CHUNK_SIZE * 4;
    int z = CHUNK_SIZE * 4;

    for (int chunkY = 10 - 1; chunkY >= 0; chunkY--) {
        auto chunkPosition = worldToChunkPosition({x, 0, z});
        chunkPosition.y = chunkY;
        auto& spawn = m_chunks.getChunk(chunkPosition);

        for (int voxelY = CHUNK_SIZE - 1; voxelY >= 0; voxelY--) {
            auto voxelPosition = toLocalVoxelPosition({x, 0, z});
            voxelPosition.y = voxelY;
            if (spawn.qGetVoxel(voxelPosition) != 0) {
                auto worldY = chunkY * CHUNK_SIZE + voxelY + 3;
                return {x, worldY * 2, z};
            }
        }
    }
    return {x, CHUNK_SIZE * 3, z};
}

std::optional<VoxelPosition> ServerWorld::tryInteract(InteractionKind kind,
                                                      const glm::vec3& position,
                                                      const glm::vec3& rotation)
{
    auto voxelPositions = getIntersectedVoxels(position, forwardsVector(rotation), 8);
    if (voxelPositions.empty()) {
        return {};
    }
    VoxelPosition& previous = voxelPositions.at(0);

    for (auto& voxelPosition : voxelPositions) {
        auto voxelId = m_chunks.getVoxel(voxelPosition);
        auto type = m_voxelData.getVoxelData(voxelId).type;
        if (type == VoxelType::Solid || type == VoxelType::Flora) {
            auto air = m_voxelData.getVoxelId(CommonVoxel::Air);
            auto stone = m_voxelData.getVoxelId(CommonVoxel::Stone);

            VoxelUpdate update;
            update.voxel = kind == InteractionKind::DigBlock ? air : stone;
            update.voxelPosition =
                kind == InteractionKind::DigBlock ? voxelPosition : previous;

            m_chunks.setVoxel(update.voxelPosition, update.voxel);
            return update.voxelPosition;
        }
        previous = voxelPosition;
    }
    return {};
}
