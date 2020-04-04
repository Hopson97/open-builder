#pragma once

#include <common/world/entity_state.h>
#include <common/world/chunk_manager.h>
#include "../renderer/chunk_renderer.h"
#include "../gl/textures.h"

class Camera;

struct LocalPlayer {
    EntityState* m_entity;
};

class ClientWorld {
  public:
    ClientWorld();

    void setPlayerId(u32 id);
    void setupData(int maxEntities);

    void tick(float dt);
    void render(const Camera& camera);

    void addEntity(u32 id, const glm::vec3& position, const glm::vec3& rotation);
    void removeEntity(u32 id);

    EntityState& getPlayer();

  private:
    std::vector<EntityState> m_entities;
    ChunkManager m_chunks;
    ChunkRenderer m_chunkRenderer;
    Chunk* chunk = nullptr;

    u32 m_playerId = 0;

    bool m_issetup = false;

    // temp
    gl::VertexArray m_vao;
    gl::Shader m_entityShader;
    gl::UniformLocation m_entityProj;
    gl::UniformLocation m_entityModel;
    gl::Texture2d m_playerTexture;
};