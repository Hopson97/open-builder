#include "vertex_array.h"

#include "../gl/gl_errors.h"
#include "mesh.h"

namespace client {
    RenderInformation::RenderInformation(GLuint handle, GLsizei indicesCount)
    :   m_handle        (handle)
    ,   m_indicesCount  (indicesCount)
    {}

    void RenderInformation::bindAndDraw()
    {
        glCheck(glBindVertexArray(m_handle));
        glCheck(glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, nullptr));
    }

    VertexArray::VertexArray()
    {
        create();
    }

    VertexArray::VertexArray(const Mesh &mesh)
    {
        create();
        create(mesh);
    }

    VertexArray::VertexArray(VertexArray &&other)
    {
        m_handle = other.m_handle;
        m_vertexBuffers = std::move(other.m_vertexBuffers);
        m_indicesCount = other.m_indicesCount;

        other.m_handle = 0;
        other.m_indicesCount = 0;
    }

    VertexArray &VertexArray::operator=(VertexArray &&other)
    {
        m_handle = other.m_handle;
        m_vertexBuffers = std::move(other.m_vertexBuffers);
        m_indicesCount = other.m_indicesCount;

        other.m_handle = 0;
        other.m_indicesCount = 0;
        return *this;
    }

    VertexArray::~VertexArray()
    {
        glCheck(glDeleteVertexArrays(1, &m_handle));
        glCheck(
            glDeleteBuffers(m_vertexBuffers.size(), m_vertexBuffers.data()));
    }

    void VertexArray::create(const Mesh &mesh)
    {
        bind();
        addVertexBuffer(3, mesh.vertices);
        addVertexBuffer(2, mesh.textureCoords);
        addVertexBuffer(3, mesh.normals);

        addIndexBuffer(mesh.indices);
    }

    void VertexArray::addVertexBuffer(int magnitude,
                                      const std::vector<GLfloat> &data)
    {
        GLuint vbo;
        glCheck(glGenBuffers(1, &vbo));
        glCheck(glBindBuffer(GL_ARRAY_BUFFER, vbo));

        glCheck(glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(GLfloat),
                             data.data(), GL_STATIC_DRAW));

        glCheck(glVertexAttribPointer(m_vertexBuffers.size(), magnitude,
                                      GL_FLOAT, GL_FALSE, 0, (GLvoid *)0));

        glCheck(glEnableVertexAttribArray(m_vertexBuffers.size()));

        m_vertexBuffers.push_back(vbo);
    }

    void VertexArray::addIndexBuffer(const std::vector<GLuint> &indices)
    {
        GLuint vbo;
        glCheck(glGenBuffers(1, &vbo));
        glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo));

        glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                             indices.size() * sizeof(GLuint), indices.data(),
                             GL_STATIC_DRAW));

        m_indicesCount = indices.size();
    }

    void VertexArray::bind() const
    {
        glCheck(glBindVertexArray(m_handle));
    }

    GLsizei VertexArray::getIndicesCount() const
    {
        return m_indicesCount;
    }

    RenderInformation VertexArray::getRenderInfo() const
    {
        return {
            m_handle, m_indicesCount
        };
    }

    void VertexArray::create()
    {
        glCheck(glGenVertexArrays(1, &m_handle));
    }
} // namespace client
