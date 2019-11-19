#pragma once

#include "../../renderer/model/mesh.h"
#include "../../renderer/model/vertex_array.h"

namespace client {
    class ChunkSection;
    /**
     * @brief Drawable representation of a chunk
     */
    struct ChunkMeshObjects final {
        VertexArray solid;
    };

    struct ChunkMesh {
        Mesh basicInfo;
        std::vector<GLfloat> basicLight;
        unsigned maxIndex = 0;
    };

    struct ChunkMeshGroup {
        ChunkMesh solidMesh;
    };

    ChunkMeshObjects bufferChunkMeshGroup(ChunkMeshGroup& group);

    ChunkMeshGroup createChunkMesh(const ChunkSection &section);
} // namespace client
