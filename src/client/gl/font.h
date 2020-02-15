#pragma once

#include "../gl/textures.h"
#include <SFML/Graphics/Font.hpp>

namespace gl {
/**
 * @brief Opengl supporting font
 *
 */
class Font final {
  public:
    void init(const std::string& fontFile, unsigned bitmapScale);
    const sf::Glyph& getGlyph(char character) const;

    float getKerning(char before, char next) const;
    float getLineHeight() const;

    void bindTexture() const;

    unsigned getTextureAtlasSize() const;
    unsigned getBitmapSize() const;

  private:
    sf::Font m_font;
    gl::Texture2d m_texture;
    unsigned m_bitmapScale = 0;
    unsigned m_imageSize = 0;
    const std::string m_charSet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGH"
                                  "IJKLMNOPQRSTUVWXYZ.,!?-+/()[]:;%&`*#=\"";
};

} // namespace gl