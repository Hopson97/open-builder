#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics/Font.hpp>
#include "../gl/textures.h"
#include "../gl/vertex_array.h"

namespace gl
{
    class Shader;
    struct UniformLocation;
} // namespace gl


class Font
{
    public:
        void init(const std::string& fontFile, unsigned bitmapScale);
        const sf::Glyph& getGlyph(char character) const;
        
        unsigned getKerning(char before, char next) const;
        unsigned getLineHeight() const;

        void bindTexture() const;

        unsigned getTextureAtlasSize() const;
        unsigned getBitmapSize() const;

    private:
        sf::Font m_font;
        gl::Texture2d m_texture;
        unsigned m_bitmapScale = 0;
        unsigned m_imageSize = 0;
        const std::string m_charSet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,!?-+/()[]:;%&`*#=\"";
};

class Text final
{
    public:
        void setFont(const Font& font);
        void setText(const std::string& string);
        void setCharSize(float size);
        void setPosition(const glm::vec3& position);

        void render(const gl::UniformLocation& location);

    private:
        void createGeometry();

        std::string m_text;
        gl::VertexArray m_vao;
        float m_scale = 0;
        const Font* m_font = nullptr;
        bool m_needsUpdate = false;

        glm::vec3 m_position;
};