#include "text.h"

#include "gl/shader.h"
#include "maths.h"

namespace {
struct Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> textureCoords;
    std::vector<GLuint> indices;
    GLuint icount = 0;
};

//Adapted from https://github.com/SFML/SFML/blob/master/src/SFML/Graphics/Text.cpp
// void addGlyphQuad and ensureGeometryUpdate

/**
 * @brief Creates a quad that contains a character, and adds to a mesh
 * 
 * @param mesh The mesh to add the character to
 * @param glyph The glyph being added
 * @param c The character being added
 * @param imageSize The size of the texture atlas
 * @param position The world position to put this char at
 * @param maxHeight The maximum height of the chars
 */
void addCharacter(Mesh &mesh, const sf::Glyph &glyph, float size, const sf::Vector2f& position)
{
    //Find the vertex positions of the the quad that will render this character
    float left = glyph.bounds.left;
    float top = glyph.bounds.top;
    float right = glyph.bounds.left + glyph.bounds.width;
    float bottom = glyph.bounds.top + glyph.bounds.height;

    // Find the texture coords in the texture
    float pad = 1.0f;
    float texLeft = (static_cast<float>(glyph.textureRect.left) - pad) / size;
    float texRight = (static_cast<float>(glyph.textureRect.left + glyph.textureRect.width) + pad) / size;
    float texTop = (static_cast<float>(glyph.textureRect.top) - pad) / size;
    float texBottom  = (static_cast<float>(glyph.textureRect.top + glyph.textureRect.height) + pad) / size;

    // Add the vertex positions to the mesh
    float scale = 1;
    mesh.vertices.insert(mesh.vertices.end(), {
         (position.x + left) / scale,  (position.y + top) / scale,
        (position.x + right) / scale, (position.y + top) / scale,
        (position.x + right) / scale, (position.y + bottom) / scale,
        (position.x + left) / scale, (position.y + bottom) / scale,

    });

    // Add the textrue coords to the mesh
    mesh.textureCoords.insert(mesh.textureCoords.end(), {
        texLeft, texTop,
        texRight, texTop,
        texRight, texBottom,
        texLeft, texBottom,
    });

    // Add indices to the mesh
    mesh.indices.push_back(mesh.icount);
    mesh.indices.push_back(mesh.icount + 1);
    mesh.indices.push_back(mesh.icount + 2);
    mesh.indices.push_back(mesh.icount + 2);
    mesh.indices.push_back(mesh.icount + 3);
    mesh.indices.push_back(mesh.icount);
    mesh.icount += 4;
}

} // namespace 


void Font::init(const std::string& fontFile, unsigned bitmapScale)
{
    m_bitmapScale = bitmapScale;
    m_font.loadFromFile(fontFile);
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


unsigned Font::getKerning(char before, char next) const
{
    return m_font.getKerning(before, next, m_bitmapScale);
}

unsigned Font::getLineHeight() const
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

//  ===============================
//      Text Class Implemenation
//
void Text::setFont(const Font& font)
{
    m_font = &font;
}

void Text::setText(const std::string& string)
{
    m_text = string;
    m_needsUpdate = true;
}

void Text::setCharSize(float size)
{
    m_scale = size;
}

void Text::setPosition(const glm::vec3& position)
{
    m_position = position;
}

void Text::render(const gl::UniformLocation& location)
{
    m_font->bindTexture();
    if (!m_font) {
        return;
    }
    if (m_needsUpdate) {
        createGeometry();
    }
    glm::mat4 modelMatrix {1.0f};
    float scale = m_scale / m_font->getBitmapSize();

    translateMatrix(modelMatrix, m_position);
    rotateMatrix(modelMatrix, {180.0f, 0.0f, 0.0f});
    scaleMatrix(modelMatrix, scale);

    gl::loadUniform(location, modelMatrix);

    m_vao.getDrawable().bindAndDraw();
}



void Text::createGeometry()
{
    m_vao.destroy();
    Mesh mesh;
    m_needsUpdate = false;

    sf::Vector2f pos{0, m_scale};
    char previous = 0;
    for (auto character : m_text) {
        pos.x += m_font->getKerning(previous, character);
        previous = character;

        //New line handler
        if (character == '\n') {
            pos.y += m_font->getLineHeight();
            pos.x = 0;
        }

        //Create a single quad for the char
        auto& glyph = m_font->getGlyph(character);
        addCharacter(mesh, glyph, m_font->getTextureAtlasSize(), pos);
        pos.x += glyph.advance;
    }

    m_vao.create();
    m_vao.bind();
    m_vao.addVertexBuffer(2, mesh.vertices);
    m_vao.addVertexBuffer(2, mesh.textureCoords);
    m_vao.addIndexBuffer(mesh.indices);
}