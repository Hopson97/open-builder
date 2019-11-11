#pragma once

#include <glad/glad.h>
#include <vector>

namespace client {
    class Mesh;

    class RenderInformation {
      public:
        RenderInformation(GLuint handle, GLsizei indicesCount);

        void bindAndDraw();

      private:
        const GLuint m_handle;
        const GLsizei m_indicesCount;
    };

    class VertexArray final {
      public:
        VertexArray();
        VertexArray(const Mesh &mesh);

        VertexArray(VertexArray &&other);
        VertexArray &operator=(VertexArray &&other);

        VertexArray(const VertexArray &other) = delete;
        VertexArray &operator=(const VertexArray &other) = delete;

        ~VertexArray();

        void create(const Mesh &mesh);
        void bind() const;

        GLsizei getIndicesCount() const;
        RenderInformation getRenderInfo() const;

      private:
        void create();

        void addVertexBuffer(int magnitude, const std::vector<GLfloat> &data);
        void addIndexBuffer(const std::vector<GLuint> &indices);

        GLuint m_handle;
        GLsizei m_indicesCount;

        std::vector<GLuint> m_vertexBuffers;
    };
} // namespace client
