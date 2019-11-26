#pragma once

#include <glad/glad.h>
#include <string>
#include <vector>
#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gl
{

/**
 * @brief Wrapper for a OpenGL shder object
 * 
 */
class Shader {
    public:
        void create(const std::string &vertexFile,
                    const std::string &fragmentFile);
        void destroy();

        void bind();

    private:
        GLuint m_handle = 0;
};

/**
 * @brief Wrapper for an OpenGL cube-mapped texture object
 * 
 */
class CubeTexture {
    public:
        void create(const std::array<std::string, 6> &textures);
        void destroy();

        void bind();

    private:
        GLuint m_handle = 0;
};

/**
 * @brief Wrapper for a regaulr OpenGL 2D texture
 * 
 */
class Texture2d {
    public:
        void create(const std::string &file);
        void destroy();

        void bind();

    private:
        GLuint m_handle = 0;
};

/**
 * @brief Wrapper for an OpenGL vertex array object (aka VAO)
 * 
 */
class VertexArray 
{
    public:
        void create();
        void destroy();

        void bind();

    private:
        std::vector<GLuint> m_bufferObjects;
        GLuint m_handle = 0;
        GLsizei m_indicesCount = 0;
};

}