#pragma once

#include "../../../renderer/model/vertex_array.h"

namespace client {
    /**
     * @brief Drawable representation of a chunk
     */
    struct ChunkMesh final {
        VertexArray solidBlocks;
        //VertexArray liquid;
        //VertexArray floral;
    };
} // namespace client
