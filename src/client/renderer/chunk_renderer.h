#pragma once

#include "../game/chunk_mesh.h"
#include "../gl/shader.h"
#include "../gl/vertex_array.h"
#include <common/maths.h>

#include <SFML/System/Clock.hpp>
#include <common/world/coordinate.h>
#include <vector>

class Camera;

/**
 * @brief A mesh that makes up the verticies and such of a chunk in the world
 */
struct ChunkRenderable final {
    ChunkPosition position;
    gl::VertexArray vao;

    // Used for debugging (Seeing vram usage)
    size_t bufferSize = 0;
};
using ChunkRenderList = std::vector<ChunkRenderable>;

/**
 * @brief Used for mostly debugging purposes, used to count number of chunks being
 * rendered, and how large the rendered chunk's buffer is
 */
struct ChunkRenderResult final {
    int chunksRendered = 0;
    int bytesInView = 0;
};

/**
 * @brief Manager class for all things chunk rendering
 */
class ChunkRenderer final {
    /**
     * @brief Used as in index for the chunk renderable array (below)
     */
    enum class ChunkRenderType {
        Solid = 0,
        Fluid = 1,
        Flora = 2,
    };

    /**
     * @brief Common shader for chunks
     */
    struct ChunkShader {
        gl::Shader program;
        gl::UniformLocation projectionViewLocation;
        gl::UniformLocation chunkPositionLocation;
    };

    /**
     * @brief Additional chunk shader utility for time based events
     */
    struct ChunkAnimatedShader final : ChunkShader {
        gl::UniformLocation timeLocation;
    };

  public:
    void init();

    /**
     * @brief Update a chunk mesh in the world
     * This function will also delete a chunk mesh given it exists
     * @param position The position of the chunk to update
     * @param meshes The meshes to be processed
     */
    void updateMesh(const ChunkPosition& position, ChunkMeshCollection&& meshes);

    /**
     * @brief Render all chunks (that are in view)
     *
     * @param camera The camera to render the chunks from
     * @param cameraInWater Is the camera in water, for rendering special effects if the
     * case
     * @return ChunkRenderResult The count of chunks rendered and their total buffer size
     * this frame
     */
    ChunkRenderResult renderChunks(const Camera& camera, bool cameraInWater);

    // Used for the debug stat view
    int getTotalChunks() const;
    int getTotalBufferSize() const;

  private:
    void deleteChunkRenderables(const ChunkPosition& position);

    ChunkShader m_shader;

    std::vector<ChunkMeshCollection> m_chunkMeshes;
    std::array<ChunkRenderList, 3> m_chunkRenderables;
};
