#include "textures.h"
#include "gl_errors.h"
#include <iostream>

namespace {
    GLuint createTexture()
    {
        GLuint handle;
        glCheck(glGenTextures(1, &handle));
        glCheck(glActiveTexture(GL_TEXTURE0));
        return handle;
    }

    void texImage2d(GLenum param, const sf::Image& image)
    {
        glCheck(glTexImage2D(param, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0,
                             GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr()));
    }

    bool bufferImage(GLenum param, const std::string& file, bool flipImage)
    {
        sf::Image img;
        if (!img.loadFromFile(file)) {
            std::cerr << "Could not load: " << file << '\n';
            return false;
        }
        if (flipImage) {
            img.flipVertically();
        }
        texImage2d(param, img);
        return true;
    }

    void destroyTexture(GLuint* texture)
    {
        glCheck(glDeleteTextures(1, texture));
        *texture = 0;
    }

} // namespace

namespace gl {

    //
    //  Cube Texture
    //
    /*
    void CubeTexture::create(const std::array<std::string, 6> &textures)
    {
        m_handle = createTexture();
        bind();

        for (int i = 0; i < 6; i++) {
            auto path = TEXTURE_PATH + "/skies/" + textures[i] + ".png";
            bufferImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, path);
        }

        glCheck(
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
    GL_LINEAR)); glCheck( glTexParameteri(GL_TEXTURE_CUBE_MAP,
    GL_TEXTURE_MAG_FILTER, GL_LINEAR));

        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                                GL_CLAMP_TO_EDGE));
        glCheck(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                                GL_CLAMP_TO_EDGE));

    }

    void CubeTexture::destroy()
    {
        destroyTexture(&m_handle);
        m_handle = 0;
    }

    void CubeTexture::bind() const
    {
        glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_handle));
    }
    */

    //
    //  Texture 2D
    //

    Texture2d::Texture2d()
        : m_handle{createTexture()}
    {
    }

    Texture2d::~Texture2d()
    {
        destroy();
    }

    Texture2d::Texture2d(Texture2d&& other)
    {
        *this = std::move(other);
    }

    Texture2d& Texture2d::operator=(Texture2d&& other)
    {
        destroy();
        m_hasTexture = other.m_hasTexture;
        m_handle = other.m_handle;
        other.reset();
        return *this;
    }

    GLuint Texture2d::createFramebufferTexture(unsigned width, unsigned height)
    {
        bind();

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        m_hasTexture = true;
        return m_handle;
    }

    void Texture2d::create(const sf::Image& image)
    {
        bind();

        texImage2d(GL_TEXTURE_2D, image);

        glCheck(glGenerateMipmap(GL_TEXTURE_2D));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR_MIPMAP_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f));
        m_hasTexture = true;
    }

    void Texture2d::create(const std::string& file, bool flipImage)
    {
        bind();

        bufferImage(GL_TEXTURE_2D, file, flipImage);

        glCheck(glGenerateMipmap(GL_TEXTURE_2D));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR_MIPMAP_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f));

        m_hasTexture = true;
    }

    void Texture2d::create(unsigned int width, unsigned int height,
                           const sf::Uint8* pixels)
    {
        if (!m_handle) {
            m_handle = createTexture();
        }
        bind();

        sf::Image img;
        img.create(width, height, pixels);
        glCheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.getSize().x, img.getSize().y,
                             0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr()));

        glCheck(glGenerateMipmap(GL_TEXTURE_2D));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR_MIPMAP_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        glCheck(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f));

        m_hasTexture = true;
    }

    void Texture2d::destroy()
    {
        destroyTexture(&m_handle);
    }

    void Texture2d::bind() const
    {
        glCheck(glBindTexture(GL_TEXTURE_2D, m_handle));
    }

    bool Texture2d::textureExists() const
    {
        return m_hasTexture;
    }

    sf::Image loadRawImageFile(const std::string& file)
    {
        sf::Image img;
        auto path = "res/" + file + ".png";

        if (!img.loadFromFile(path)) {
            std::cerr << "Could not load: " << file << '\n';
            return sf::Image();
        }

        return img;
    }

    void Texture2d::reset()
    {
        m_handle = 0;
        m_hasTexture = false;
    }

    //
    //  Texture Array
    //
    TextureArray::TextureArray()
        : m_handle{createTexture()}
    {
    }

    TextureArray::~TextureArray()
    {
        destroy();
    }

    TextureArray::TextureArray(TextureArray&& other)
    {
        *this = std::move(other);
    }

    TextureArray& TextureArray::operator=(TextureArray&& other)
    {
        destroy();
        m_handle = other.m_handle;
        m_textureCount = other.m_textureCount;
        m_maxTextures = other.m_maxTextures;
        m_textureSize = other.m_textureSize;
        other.reset();
        return *this;
    }

    void TextureArray::create(GLsizei numTextures, GLsizei textureSize)
    {
        if (!m_handle) {
            m_handle = createTexture();
        }
        bind();

        m_maxTextures = numTextures;
        m_textureSize = textureSize;

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, textureSize, textureSize,
                     numTextures, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }

    GLuint TextureArray::addTexture(const std::string& file)
    {
        sf::Image image;
        if (!image.loadFromFile(file + ".png")) {
            // Create a error image
            image.create(m_textureSize, m_textureSize);
            for (GLuint y = 0; y < m_textureSize; y++) {
                for (GLuint x = 0; x < m_textureSize; x++) {
                    uint8_t r = static_cast<uint8_t>(rand() % 255);
                    uint8_t g = static_cast<uint8_t>(rand() % 255);
                    uint8_t b = static_cast<uint8_t>(rand() % 255);
                    image.setPixel(x, y, {r, g, b});
                }
            }
        };

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, m_textureCount, m_textureSize,
                        m_textureSize, 1, GL_RGBA, GL_UNSIGNED_BYTE,
                        image.getPixelsPtr());

        // Generate Mipmap
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);
        glTexParameterf(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_LOD_BIAS, -1);

        return m_textureCount++;
    }

    void TextureArray::destroy()
    {
        destroyTexture(&m_handle);
        m_textureCount = 0;
        m_maxTextures = 0;
        m_textureSize = 0;
    }

    void TextureArray::bind() const
    {
        glCheck(glBindTexture(GL_TEXTURE_2D_ARRAY, m_handle));
    }

    void TextureArray::reset()
    {
        m_handle = 0;
        m_textureCount = 0;
        m_maxTextures = 0;
        m_textureSize = 0;
    }

} // namespace gl
