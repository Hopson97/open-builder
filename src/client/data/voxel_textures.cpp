#include "voxel_textures.h"

void VoxelTextures::init(GLsizei numTextures, GLsizei textureSize)
{
    m_textures.create(numTextures, textureSize);
    m_textureMap.reserve(numTextures);
}

void VoxelTextures::destroy()
{
    m_textures.destroy();
    m_textureMap.clear();
}

void VoxelTextures::bind() const
{
    m_textures.bind();
}

GLuint VoxelTextures::getTextureId(const std::string &name)
{
    auto itr = m_textureMap.find(name);
    if (itr == m_textureMap.end()) {
        auto id = m_textures.addTexture(name);
        m_textureMap.emplace(name, id);
        return id;
    }
    return itr->second;
}
