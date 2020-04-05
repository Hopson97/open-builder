#include "client_world.h"
#include "../gl/primitive.h"
#include "../maths.h"
#include "../renderer/camera.h"
#include "chunk_mesh_generation.h"
#include <common/debug.h>

ClientWorld::ClientWorld()
{
    m_entities.resize(1024);
    m_chunkRenderer.init();

    m_vao = makeCubeVertexArray(1, 2, 1);
    m_entityShader.create("static", "static");
    m_entityShader.bind();
    m_playerTexture.create("res/skins/player.png", false);
    m_entityProj = m_entityShader.getUniformLocation("projectionViewMatrix");
    m_entityModel = m_entityShader.getUniformLocation("modelMatrix");
}

void ClientWorld::setPlayerId(u32 id)
{
    std::cout << "Got player ID: " << id << std::endl;
    m_playerId = id;
}

void ClientWorld::setupData(int maxEntities)
{
    m_issetup = true;
}

void ClientWorld::tick(float dt)
{
    if (!m_issetup) {
        return;
    }
    auto& player = getPlayer();
    player.position += player.velocity * dt;
    player.velocity *= 0.90 * dt;
}

void ClientWorld::render(const Camera& camera)
{
    if (!m_issetup) {
        return;
    }
    m_chunkRenderer.renderChunks(camera, false);

    m_entityShader.bind();
    m_playerTexture.bind();
    gl::loadUniform(m_entityProj, camera.getProjectionView());

    auto d = m_vao.getDrawable();
    d.bind();
    for (u32 i = 1; i < m_entities.size(); i++) {
        auto& entity = m_entities[i];
        if (entity.active && i != m_playerId) {
            glm::mat4 model{1.0f};
            translateMatrix(model, entity.position);
            gl::loadUniform(m_entityModel, model);
            d.draw();
        }
    }
}

void ClientWorld::addEntity(u32 id, const glm::vec3& position, const glm::vec3& rotation)
{
    std::cout << "Got entity ID: " << id << std::endl;
    assert(id <= m_entities.size());
    m_entities[id].active = true;
    m_entities[id].position = position;
    m_entities[id].rotation = rotation;
}

void ClientWorld::updateEntity(u32 id, const glm::vec3& position,
                               const glm::vec3& rotation)
{
    assert(id <= m_entities.size());
    if (!m_entities[id].active) {
        return;
    }
    m_entities[id].position = position;
    m_entities[id].rotation = rotation;
}

void ClientWorld::removeEntity(u32 id)
{
    assert(id <= m_entities.size());
    m_entities[id].active = false;
}

EntityState& ClientWorld::getPlayer()
{
    return m_entities[m_playerId];
}

u32 ClientWorld::getPlayerId() const
{
    return m_playerId;
}
