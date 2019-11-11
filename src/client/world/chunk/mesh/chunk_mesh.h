#pragma once

#include "../../../renderer/model/vertex_array.h"

namespace client {
    /**
     * @brief Drawable representation of a chunk
     *
     */
    class ChunkMesh final {
      public:
        ChunkMesh();

        void render();

        // private:
        VertexArray array;
    };
} // namespace client
