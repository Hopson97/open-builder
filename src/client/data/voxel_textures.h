#pragma once

#include "../gl/textures.h"
#include <unordered_map>

class VoxelTextures {
  public:
    void init(GLsizei numTextures, GLsizei textureSize);
    void destroy();
    void bind() const;

    GLuint getTextureId(const std::string &name);

  private:
    gl::TextureArray m_textures;
    std::unordered_map<std::string, GLuint> m_textureMap;
};