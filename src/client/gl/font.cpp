#include "font.h"

#include <cassert>

namespace gl {

void Font::init(const std::string& fontFile, unsigned bitmapScale)
{
    m_bitmapScale = bitmapScale;
    if (!m_font.loadFromFile(fontFile)) {
        throw std::runtime_error("Unable to load font from file...");
    }
    for (auto character : m_charSet) {
        m_font.getGlyph(character, bitmapScale, false);
    }
    const sf::Texture& temp = m_font.getTexture(bitmapScale);
    sf::Image bitmap = temp.copyToImage();
    assert(bitmap.getSize().x == bitmap.getSize().y);
    m_imageSize = bitmap.getSize().x;
    m_texture.create(bitmap);
}

const sf::Glyph& Font::getGlyph(char character) const
{
    return m_font.getGlyph(character, m_bitmapScale, false);
}

float Font::getKerning(char before, char next) const
{
    return m_font.getKerning(before, next, m_bitmapScale);
}

float Font::getLineHeight() const
{
    return m_font.getLineSpacing(m_bitmapScale);
}

void Font::bindTexture() const
{
    m_texture.bind();
}

unsigned Font::getTextureAtlasSize() const
{
    return m_imageSize;
}

unsigned Font::getBitmapSize() const
{
    return m_bitmapScale;
}

} // namespace gl