#include "world.h"

#include <common/coordinate.h>
#include <common/debug.h>
#include <iostream>

namespace client {
    World::World()
        : m_emptyChunk({-1, -1}, *this)
    {
        ChunkSection air(-1, 0, -1, *this);
        m_emptyChunk.addSection(1, std::move(air));
        std::cout << "CHUNK: " << m_emptyChunk.hasAllData() << std::endl;

        for (int z = 0; z < WORLD_SIZE; ++z) {
            for (int x = 0; x < WORLD_SIZE; ++x) {
                m_chunks.emplace_back(ChunkPosition(x, z), *this);
            }
        }
    }

    void World::render(Renderer &renderer)
    {
        for (auto &chunk : m_chunks) {
            chunk.render(renderer);
        }
    }

    Block World::getBlock(const BlockPosition &blockPosition)
    {
        auto chunkPosition = worldBlockToChunkPosition(blockPosition);
        if (blockPosition.x < 0 ||
            blockPosition.x >= (WORLD_SIZE * CHUNK_SIZE) ||
            blockPosition.z < 0 ||
            blockPosition.z >= (WORLD_SIZE * CHUNK_SIZE) ||
            blockPosition.y < 0 || chunkPosition.x < 0 ||
            chunkPosition.x >= WORLD_SIZE || chunkPosition.z < 0 ||
            chunkPosition.z >= WORLD_SIZE) {
            return BlockType::Air;
        }

        return getChunk(chunkPosition)
            .getBlock(worldBlockToChunkBlockPosition(blockPosition));
    }

    Chunk &World::getChunk(const ChunkPosition &position)
    {
        if (position.x < 0 || position.x >= WORLD_SIZE || position.z < 0 ||
            position.z >= WORLD_SIZE) {
            return m_emptyChunk;
        }
        return m_chunks[(position.z * WORLD_SIZE + position.x)];
    }

    void World::update([[maybe_unused]] Entity &player)
    {
        for (auto &chunk : m_chunks) {
            if (chunk.hasAllData()) {
                if ([&]() {
                        for (int z = -1; z <= 1; z++) {
                            for (int x = -1; x <= 1; x++) {
                                const Chunk &c =
                                    getChunk({chunk.getPosition().x + x,
                                              chunk.getPosition().z + z});
                                if (!c.hasAllData()) {
                                    return false;
                                }
                            }
                        }
                        return true;
                    }()) {

                    if (chunk.tryCreateMesh()) {
                        break;
                    }
                }
            }
        }
    }
} // namespace client

/*
static int distance = 1;
bool meshMade = false;
[&]() {
    for (int z = -distance; z <= distance; z++) {
        for (int x = -distance; x <= distance; x++) {
            int chunkX = (player.transform.position.x) / CHUNK_SIZE + x;
            int chunkZ = (player.transform.position.z) / CHUNK_SIZE + z;
            auto itr = chunks.find({chunkX, 0, chunkZ});
            if (itr != chunks.cend()) {
                auto &chunk = itr->second;
                if (chunk.flag == ChunkSection::Flag::NeedsNewMesh) {
                    ChunkMeshBuilder builder(chunk);
                    chunkMeshes[itr->first] = builder.createMesh();
                    chunk.flag = ChunkSection::Flag::None;
                    meshMade = true;
                    return;
                }
            }
        }
    }
}();

std::cout << distance << std::endl;
if (!meshMade) {
    distance++;
}
*/