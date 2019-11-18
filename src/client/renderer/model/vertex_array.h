#pragma once

#include <glad/glad.h>
#include <vector>

#include "../gl/gl_errors.h"

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

        bool isCreated() const;

        GLsizei getIndicesCount() const;
        RenderInformation getRenderInfo() const;

        template <typename T>
        void addVertexBuffer(int magnitude, const std::vector<T> &data,
                             GLenum type)
        {
            GLuint vbo;
            glCheck(glGenBuffers(1, &vbo));
            glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));

            glCheck(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T),
                                 data.data(), GL_STATIC_DRAW));

            glCheck(glVertexAttribPointer(m_vertexBuffers.size(), magnitude,
                                          type, GL_FALSE, 0, (GLvoid *)0));

            glCheck(glEnableVertexAttribArray(m_vertexBuffers.size()));

            m_vertexBuffers.push_back(vbo);
        }

      private:
        void create();

        void addIndexBuffer(const std::vector<GLuint> &indices);

        GLuint m_handle = 0;
        GLsizei m_indicesCount = 0;

        std::vector<GLuint> m_vertexBuffers;
    };
} // namespace client
