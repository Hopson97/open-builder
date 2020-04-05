#include "client_world.h"
#include "../client_config.h"
#include "../gl/primitive.h"
#include "../maths.h"
#include "../renderer/camera.h"
#include "chunk_mesh_generation.h"
#include <common/debug.h>

GLuint VoxelTextureMap::getTextureId(const std::string& name)
{
    auto itr = textureMap.find(name);
    if (itr == textureMap.end()) {
        GLuint textureId = textures.addTexture(name);
        textureMap.emplace(name, textureId);
        return textureId;
    }
    return itr->second;
}

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

    // Update chunks
    i32 count = 0;
    for (auto itr = m_chunkUpdates.begin(); itr != m_chunkUpdates.end();) {
        if (m_chunks.hasNeighbours(*itr)) {
            const Chunk& chunk = m_chunks.getChunk(*itr);
            ChunkMeshCollection meshes = makeChunkMesh(chunk, m_voxelData);
            m_chunkRenderer.updateMesh(*itr, std::move(meshes));
            itr = m_chunkUpdates.erase(itr);
            if (count++ > 3) {
                break;
            }
        }
        else {
            itr++;
        }
    }
}

void ClientWorld::render(const Camera& camera)
{
    if (!m_issetup) {
        return;
    }
    m_voxelTextures.textures.bind();

    // Render chunks, getting the block at the player position for """effects"""
    auto playerVoxel = m_chunks.getVoxel(toVoxelPosition(getPlayer().position));
    auto waterId = m_voxelData.getVoxelId(CommonVoxel::Water);
    m_chunkRenderer.renderChunks(camera, playerVoxel == waterId);

    // Render entities
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

void ClientWorld::setVoxelTextureCount(int count)
{
    // 1. Need to somehow work out the exact amount of textures needed
    // 2. Need to pass in the actual texture pack resolution (right now it is hardcoded
    // 16)
    m_voxelTextures.textures.create(count * 3, 16);
}

void ClientWorld::addVoxelType(VoxelData&& voxel)
{
    const std::string texturePath =
        "texture_packs/" + ClientConfig::get().texturePack + "/voxels/";

    std::string& top = voxel.topTexture;
    std::string& side = voxel.sideTexture;
    std::string& bottom = voxel.topTexture;

    voxel.topTextureId = m_voxelTextures.getTextureId(texturePath + top);
    voxel.sideTextureId = m_voxelTextures.getTextureId(texturePath + side);
    voxel.bottomTextureId = m_voxelTextures.getTextureId(texturePath + bottom);

    m_voxelData.addVoxelData(std::move(voxel));
}

void ClientWorld::initialiseCommonVoxels()
{
    m_voxelData.initCommonVoxelTypes();
}

bool ClientWorld::isVoxelInteractable(const VoxelPosition& position)
{
    auto voxelId = m_chunks.getVoxel(position);
    auto type = m_voxelData.getVoxelData(voxelId).type;

    return type == VoxelType::Solid || type == VoxelType::Flora;
}

bool ClientWorld::hasChunk(const ChunkPosition& position) const
{
    return m_chunks.hasChunk(position);
}

void ClientWorld::createChunkFromCompressed(const ChunkPosition& position,
                                            const CompressedVoxels& voxels)
{
    Chunk& chunk = m_chunks.addChunk(position);
    chunk.voxels = decompressVoxelData(voxels);
    m_chunkUpdates.push_back(position);
}

EntityState& ClientWorld::getPlayer()
{
    return m_entities[m_playerId];
}

u32 ClientWorld::getPlayerId() const
{
    return m_playerId;
}
