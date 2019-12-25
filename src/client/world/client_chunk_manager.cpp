#include "client_chunk_manager.h"

#include "chunk_mesh_generation.h"

void ClientChunkManager::addChunk(const ChunkPosition &position,
                                  Chunk::Blocks &&blocks)
{
    Chunk &chunk = m_chunkContainer.addChunk(position);
    chunk.blocks = std::move(blocks);
    m_updates.insert(position);
}

void ClientChunkManager::updateMeshes()
{
    for (auto itr = m_updates.begin(); itr != m_updates.end();) {
        auto &position = *itr;
        if (findChunkDrawable(position) == -1 &&
            m_chunkContainer.hasNeighbours(position)) {

            m_chunkRenderables.bufferables.push_back(
                makeChunkMesh(m_chunkContainer.getChunk(position)));
            itr = m_updates.erase(itr);
        }
        else {
            itr++;
        }
    }
}

void ClientChunkManager::renderChunks()
{
    for (auto &chunk : m_chunkRenderables.bufferables) {
        m_chunkRenderables.drawables.push_back(chunk.createBuffer());
        m_chunkRenderables.positions.push_back(chunk.position);
    }
    m_chunkRenderables.bufferables.clear();

    for (auto &chunk : m_chunkRenderables.drawables) {
        chunk.getDrawable().bindAndDraw();
    }
}

void ClientChunkManager::destroyAllMeshes()
{
    for (auto &chunk : m_chunkRenderables.drawables) {
        chunk.destroy();
    }
}

int ClientChunkManager::findChunkDrawable(const ChunkPosition &position)
{
    for (int i = 0; i < static_cast<int>(m_chunkRenderables.positions.size());
         i++) {
        if (m_chunkRenderables.positions[i] == position) {
            return i;
        }
    }
    return -1;
}