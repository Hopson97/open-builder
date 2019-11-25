#include "gl_vertex_array.h"

VertexArrayContainer createVertexArray()
{
    VertexArrayContainer container;
    glCheck(glGenVertexArrays(1, &container.object.handle));
    return container;
}

void destroyVertexArray(VertexArrayContainer *vertexArray)
{
    glCheck(glDeleteVertexArrays(1, &vertexArray->object.handle));
    glCheck(glDeleteBuffers(vertexArray->bufferObjects.size(),
                            vertexArray->bufferObjects.data()));
    vertexArray->bufferObjects.clear();
    vertexArray->object.handle = 0;
    vertexArray->indicesCount = 0;
}

void bindVertexArray(VertexArray array)
{
    glCheck(glBindVertexArray(array.handle));
}

void drawElements(VertexArray array, GLsizei indices)
{
    glCheck(glBindVertexArray(array.handle));
    glCheck(glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, nullptr));
}

void VertexArrayContainer::addIndexBuffer(
                    const std::vector<GLuint> &indices)
{
    bindVertexArray(object);

    GLuint elementBuffer;
    glCheck(glGenBuffers(1, &elementBuffer));
    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer));

    glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         indices.size() * sizeof(GLuint), indices.data(),
                         GL_STATIC_DRAW));

    bufferObjects.push_back(elementBuffer);
    indicesCount = indices.size();
}