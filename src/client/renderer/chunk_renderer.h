#pragma once

#include "../gl/shader.h"
#include "../gl/vertex_array.h"
#include "../maths.h"
#include "../world/chunk_mesh.h"

#include <SFML/System/Clock.hpp>
#include <common/world/coordinate.h>
#include <vector>

struct ChunkRenderable {
    ChunkPosition position;
    gl::VertexArray vao;

    // Used for debugging (Seeing vram usage)
    size_t bufferSize;
};

class ChunkRenderer {
    struct ChunkShader {
        gl::Shader program;
        gl::UniformLocation projectionViewLocation;
        gl::UniformLocation chunkPositionLocation;
    };

    struct ChunkAnimatedShader : ChunkShader {
        gl::UniformLocation timeLocation;
    };

  public:
    void init();
    void updateMesh(const ChunkPosition& position, ChunkMeshCollection&& meshes);

    void renderChunks(const glm::vec3& cameraPosition, const ViewFrustum& frustum,
                      const glm::mat4& projectionViewMatrix);

  private:
    void deleteChunkRenderables(const ChunkPosition& position);


    ChunkShader m_solidShader;
    ChunkAnimatedShader m_floraShader;
    ChunkAnimatedShader m_fluidShader;

    std::vector<ChunkMeshCollection> m_chunkMeshes;
    std::vector<ChunkRenderable> m_solidDrawables;
    std::vector<ChunkRenderable> m_fluidDrawables;
    std::vector<ChunkRenderable> m_floraDrawables;

    sf::Clock m_animationTimer;
};