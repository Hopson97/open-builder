#include "world.h"

#include <common/coordinate.h>
#include <common/debug.h>
#include <iostream>

namespace client {
    World::World()
    {
        for (int z = 0; z < WORLD_SIZE; ++z) {
            for (int x = 0; x < WORLD_SIZE; ++x) {
                m_chunks.emplace_back(ChunkPosition(x, z), *this);
            }
        }
    }

    void World::addChunk(ChunkSection section)
    {
        getChunk({section.getPosition().x, section.getPosition().z})
            .addSection(std::move(section));
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
            chunkPosition.x > WORLD_SIZE || chunkPosition.z < 0 ||
            chunkPosition.z >= WORLD_SIZE) {
            return BlockType::Air;
        }

        return getChunk(chunkPosition)
            .getBlock(worldBlockToChunkBlockPosition(blockPosition));
    }

    const Chunk &World::getChunk(const ChunkPosition &position) const
    {
        return m_chunks.at(position.z * WORLD_SIZE + position.x);
    }

    Chunk &World::getChunk(const ChunkPosition &position)
    {
        return m_chunks[(position.z * WORLD_SIZE + position.x)];
    }

    void World::update([[maybe_unused]] Entity &player)
    {
        for (auto &chunk : m_chunks) {
            /*
            auto p = chunk.getPosition();
            ChunkPosition n1(p.x, p.z - 1);
            ChunkPosition n2(p.x, p.z + 1);
            ChunkPosition n3(p.x - 1, p.z);
            ChunkPosition n4(p.x + 1, p.z);


            if (n1.x >= 0 || n1.x < WORLD_SIZE) {
                if (getChunk(n1).countSections() == 0) {
                    continue;
                }
            }
            if (n2.x >= 0 || n2.x < WORLD_SIZE) {
                if (getChunk(n2).countSections() == 0) {
                    continue;
                }
            }
            if (n3.x >= 0 || n3.x < WORLD_SIZE) {
                if (getChunk(n3).countSections() == 0) {
                    continue;
                }
            }
            if (n4.x >= 0 || n4.x < WORLD_SIZE) {
                if (getChunk(n4).countSections() == 0) {
                    continue;
                }
            }
*/
            if (chunk.createMesh()) {
                break;
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