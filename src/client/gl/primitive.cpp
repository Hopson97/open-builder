#include "primitive.h"

gl::VertexArray makeCubeVertexArray(GLfloat width, GLfloat height, GLfloat depth)
{
    float w = width;
    float h = height;
    float d = depth;
    std::vector<GLfloat> vertices = {
        // Front
        w, h, d, 0, h, d, 0, 0, d, w, 0, d,
        // Left
        0, h, d, 0, h, 0, 0, 0, 0, 0, 0, d,
        // Back
        0, h, 0, w, h, 0, w, 0, 0, 0, 0, 0,
        // Right
        w, h, 0, w, h, d, w, 0, d, w, 0, 0,
        // Top
        w, h, 0, 0, h, 0, 0, h, d, w, h, d,
        // Bottom
        0, 0, 0, w, 0, 0, w, 0, d, 0, 0, d,
        // this just stops clang format from making the array long
    };
    std::vector<GLuint> indices;
    for (int itr = 0, i = 0; itr < 6; itr++) {
        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + 2);
        indices.push_back(i + 2);
        indices.push_back(i + 3);
        indices.push_back(i);
        i += 4;
    }
    std::vector<GLfloat> textureCoords;

    for (int i = 0; i < 6; i++) {
        textureCoords.insert(textureCoords.end(),
                             {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f});
    }

    gl::VertexArray vao;
    vao.bind();
    vao.addVertexBuffer(3, vertices);
    vao.addVertexBuffer(2, textureCoords);
    vao.addIndexBuffer(indices);

    return vao;
}

gl::VertexArray makeWireCubeVertexArray(GLfloat width, GLfloat height, GLfloat depth)
{
    float w = width;
    float h = height;
    float d = depth;
    std::vector<GLfloat> vertices = {
        // Front
        w, h, d, 0, h, d, 0, 0, d, w, 0, d,
        // Back
        w, h, 0, 0, h, 0, 0, 0, 0, w, 0, 0
        // this just stops clang format from making the array long
    };
    std::vector<GLuint> indices = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6,
                                   6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};

    gl::VertexArray vao;
    vao.bind();
    vao.addVertexBuffer(3, vertices);
    vao.addIndexBuffer(indices);

    return vao;
}

gl::VertexArray makeQuadVertexArray(GLfloat relativeWidth, GLfloat relativeHeight)
{
    auto w = relativeWidth;
    auto h = relativeHeight;
    std::vector<GLfloat> vertices = {
        0, 0, w, 0, w, h, 0, h,
    };

    std::vector<GLuint> indices = {0, 1, 2, 2, 3, 0};

    std::vector<GLfloat> textureCoords = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

    gl::VertexArray vao;
    vao.bind();
    vao.addVertexBuffer(2, vertices);
    vao.addVertexBuffer(2, textureCoords);
    vao.addIndexBuffer(indices);
    return vao;
}

gl::VertexArray makeScreenQuadVertexArray()
{
    std::vector<GLfloat> vertices = {
        -1.0f, -1.f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f,
    };

    std::vector<GLuint> indices = {0, 1, 2, 2, 3, 0};

    std::vector<GLfloat> textureCoords = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f};

    gl::VertexArray vao;
    vao.bind();
    vao.addVertexBuffer(2, vertices);
    vao.addVertexBuffer(2, textureCoords);
    vao.addIndexBuffer(indices);
    return vao;
}
