#pragma once

#include "gl/texture2d.h"
#include "gl/vertex_array.h"
#include <string>

namespace client {
    class TexturedModel final {
      public:
        TexturedModel();
        TexturedModel(const std::string &modelFileName,
                      const std::string &textureFileName);

        TexturedModel(const Mesh &mesh, const std::string &textureFileName);

        TexturedModel(TexturedModel &&other);
        TexturedModel &operator=(TexturedModel &&other);

        TexturedModel(const TexturedModel &other) = delete;
        TexturedModel &operator=(const TexturedModel &other) = delete;

        ~TexturedModel() = default;

        void create(const std::string &modelFileName,
                    const std::string &textureFileName);
        void create(const Mesh &mesh, const std::string &textureFileName);

        void bind() const;

        GLsizei getIndicesCount() const;

      private:
        VertexArray m_model;
        Texture2D m_texture;
    };
} // namespace client
