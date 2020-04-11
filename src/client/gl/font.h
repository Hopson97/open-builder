#pragma once

#include "../gl/textures.h"
#include <SFML/Graphics/Font.hpp>
#include <unordered_map>
#include <unordered_set>

namespace gl {

    struct FontTexture {
        unsigned size = 0;
        gl::Texture2d texture;
    };
    /**
     * @brief Opengl supporting font
     *
     */
    class Font final {
      public:
        void init(const std::string& fontFile);
        const sf::Glyph& getGlyph(char character, unsigned textSize) const;

        float getKerning(char before, char next, unsigned textSize) const;
        float getLineHeight(unsigned textSize) const;

        void bindTexture(unsigned textSize) const;

        unsigned getTextureAtlasSize() const;
        unsigned getBitmapSize() const;

        const FontTexture& getFontTexture(int fontSize);

      private:
        std::unordered_map<int, FontTexture> m_textures;
        sf::Font m_font;
        const std::string m_charSet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGH"
                                      "IJKLMNOPQRSTUVWXYZ.,!?-+/()[]:;%&`*#=\"";
    };

} // namespace gl
