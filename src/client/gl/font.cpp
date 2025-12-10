#include "font.h"

#include <cassert>
#include <iostream>

namespace gl {

    Font::Font(const std::string& fontFile)
        : m_font(fontFile)
    {
    }

    const sf::Glyph& Font::getGlyph(char character, unsigned textSize) const
    {
        return m_font.getGlyph(character, textSize, false);
    }

    float Font::getKerning(char before, char next, unsigned textSize) const
    {
        return m_font.getKerning(before, next, textSize);
    }

    float Font::getLineHeight(unsigned textSize) const
    {
        return m_font.getLineSpacing(textSize);
    }

    void Font::bindTexture(unsigned textSize) const
    {
        auto itr = m_textures.find(textSize);
        if (itr != m_textures.end()) {
            m_textures.at(textSize).texture.bind();
        }
    }

    const FontTexture& Font::getFontTexture(int textSize)
    {
        auto itr = m_textures.find(textSize);
        if (itr != m_textures.end()) {
            return m_textures.at(textSize);
        }

        // @TODO See if this can be done with having to get the glyph of every char
        for (auto character : m_charSet) {
            getGlyph(character, textSize);
        }

        const sf::Texture& temp = m_font.getTexture(textSize);
        sf::Image bitmap = temp.copyToImage();
        assert(bitmap.getSize().x == bitmap.getSize().y);

        FontTexture ft;
        ft.texture.create(bitmap);
        ft.size = bitmap.getSize().x;

        const auto& texture = m_textures.emplace(textSize, std::move(ft));
        return texture.first->second;
    }

} // namespace gl
