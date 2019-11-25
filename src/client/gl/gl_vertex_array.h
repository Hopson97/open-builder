#pragma once

#include <glad/glad.h>
#include <type_traits>
#include <vector>

#include "gl_errors.h"

enum class GLType {
    UnsignedInt = GL_UNSIGNED_INT,
    Float = GL_FLOAT,
    Byte = GL_BYTE,
};

enum class DrawStyle {
    Static = GL_STATIC_DRAW,
};

struct VertexArray {
    GLuint handle = 0;
};

struct VertexArrayContainer {
    std::vector<GLuint> bufferObjects;
    VertexArray object;
    GLsizei indicesCount = 0;

    void addIndexBuffer(const std::vector<GLuint> &indices);

    template <typename T>
    void addVertexBuffer(int bufferMagntitude,
                        const std::vector<T> &bufferData,
                        DrawStyle bufferDrawStyle, GLType bufferType);
};

VertexArrayContainer createVertexArray();
void destroyVertexArray(VertexArrayContainer *vertexArray);

void bindVertexArray(VertexArray array);
void drawElements(VertexArray array, GLsizei indices);

template <typename T>
void VertexArrayContainer::addVertexBuffer(int bufferMagntitude,
                     const std::vector<T> &bufferData,
                     DrawStyle bufferDrawStyle, GLType bufferType)
{
    bindVertexArray(object);

    GLuint vertexBuffer;
    glCheck(glGenBuffers(1, &vertexBuffer));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));

    glCheck(glBufferData(GL_ARRAY_BUFFER, bufferData.size() * sizeof(T),
                         bufferData.data(),
                         static_cast<GLenum>(bufferDrawStyle)));

    glCheck(glVertexAttribPointer(bufferObjects.size(), bufferMagntitude,
        static_cast<GLenum>(bufferType), GL_FALSE, 0, (GLvoid *)0));

    glCheck(glEnableVertexAttribArray(bufferObjects.size()));

    bufferObjects.push_back(vertexBuffer);
}
