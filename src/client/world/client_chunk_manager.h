#pragma once

#include <common/world/chunk_manager.h>
#include <unordered_set>
#include <vector>

#include "../gl/gl_object.h"
#include "chunk_mesh.h"

class ClientChunkManager {
  public:
    void addChunk(const ChunkPosition &position, Chunk::Blocks &&blocks);
    void updateMeshes();
    void renderChunks();
    void destroyAllMeshes();

  private:
    int findChunkDrawable(const ChunkPosition &position);

    struct {
        std::vector<ChunkMesh> bufferables;
        std::vector<ChunkPosition> positions;
        std::vector<gl::VertexArray> drawables;
    } m_chunkRenderables;

    std::unordered_set<ChunkPosition, ChunkPositionHash> m_updates;
    ChunkManager m_chunkContainer;
};