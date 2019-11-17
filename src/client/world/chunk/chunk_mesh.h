#pragma once

#include "../../renderer/model/mesh.h"
#include "../../renderer/model/vertex_array.h"

namespace client {
    /**
     * @brief Drawable representation of a chunk
     */
    struct ChunkDrawable final {
        VertexArray solidBlocks;
        // VertexArray liquid;
        // VertexArray floral;
    };

    struct ChunkMesh {
        struct MeshGroup {
            Mesh mesh;
            std::vector<GLfloat> basicLight;
        };
        MeshGroup solidBlockMesh;
    };
} // namespace client
