#pragma once

#include <array>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <vector>

namespace gl {

/**
 * @brief A uniform location in a shader
 */
struct UniformLocation {
    GLuint ptr = 0;
};

/**
 * @brief Wrapper for a OpenGL shder object
 */
class Shader {
  public:
    void create(const std::string &vertexFile, const std::string &fragmentFile);
    void destroy();
    void bind() const;

    UniformLocation getUniformLocation(const char *name);

  private:
    GLuint m_handle = 0;
};

/**
 * @brief Wrapper for an OpenGL cube-mapped texture object
 */
class CubeTexture {
  public:
    void create(const std::array<std::string, 6> &textures);
    void destroy();
    void bind() const;

  private:
    GLuint m_handle = 0;
};

/**
 * @brief Wrapper for a regaulr OpenGL 2D texture
 */
class Texture2d {
  public:
    void create(const std::string &file);
    void destroy();
    void bind() const;

  private:
    GLuint m_handle = 0;
};

class Drawable {
  public:
    Drawable(GLuint vao, GLsizei indices);

    void drawElements() const;

  private:
    const GLuint m_handle = 0;
    const GLsizei m_indicesCount = 0;
};

/**
 * @brief Wrapper for an OpenGL vertex array object (aka VAO)
 */
class VertexArray {
  public:
    void create();
    void destroy();
    void bind() const;

    Drawable getDrawable() const;

    void addVertexBuffer(int magnitude, const std::vector<GLfloat> &data);
    void addIndexBuffer(const std::vector<GLuint> &indices);

  private:
    std::vector<GLuint> m_bufferObjects;
    GLuint m_handle = 0;
    GLsizei m_indicesCount = 0;
};

// Functons for shaders
void loadUniform(UniformLocation location, const glm::vec3 &vector);
void loadUniform(UniformLocation location, const glm::mat4 &matrix);

/**
 * @brief Class for managing the lifetime of an OpenGL object
 *
 * @tparam GLObject The object type to create
 */
template <typename GLObject> class Object {
  public:
    template <typename... Args> void create(Args &&... args)
    {
        m_object.create(std::forward(args)...);
    }

    void destroy()
    {
        m_object.destroy();
    }

    void bind()
    {
        m_object.bind();
    }

    GLObject *operator->()
    {
        return &m_object;
    }

    ~Object()
    {
        destroy();
    }

    Object() = default;
    Object(const Object &object) = delete;
    Object &operator=(const Object &object) = delete;

  private:
    GLObject m_object;
};

} // namespace gl