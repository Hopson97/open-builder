#include "text_component.h"

#include "../../gl/font.h"
#include "../../maths.h"
#include "../../renderer/gui_shader.h"

namespace {
struct Mesh {
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> textureCoords;
    std::vector<GLuint> indices;
    GLuint icount = 0;
};

// Adapted from https://github.com/SFML/SFML/blob/master/src/SFML/Graphics/Text.cpp
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
void addCharacter(Mesh& mesh, const sf::Glyph& glyph, float size,
                  const sf::Vector2f& position)
{
    // Shorthand
    const auto& textureRect = glyph.textureRect;
    // Find the vertex positions of the the quad that will render this character
    float left = glyph.bounds.left;
    float top = glyph.bounds.top;
    float right = glyph.bounds.left + glyph.bounds.width;
    float bottom = glyph.bounds.top + glyph.bounds.height;

    // Find the texture coords in the texture
    float pad = 1.0f;
    float texLeft = (static_cast<float>(textureRect.left) - pad) / size;
    float texRight =
        (static_cast<float>(textureRect.left + textureRect.width) + pad) / size;
    float texTop = (static_cast<float>(textureRect.top) - pad) / size;
    float texBottom =
        (static_cast<float>(textureRect.top + textureRect.height) + pad) / size;

    // clang-format off
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
    // clang-format on

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

namespace gui {

void TextComponent::setPosition(const GuiDimension& position)
{
    m_position = position;
    m_isGeometryUpdateNeeded = true;
}

void TextComponent::setSize(const GuiDimension&)
{
    // Intentionally empty
}

void TextComponent::setFontSize(float size)
{
    m_fontSize = size;
    m_isGeometryUpdateNeeded = true;
}

void TextComponent::setText(const std::string& text)
{
    // todo maybe std::move, need to check if possible
    m_text = text;
    m_isGeometryUpdateNeeded = true;
}

void TextComponent::render(const gl::Font& font, GuiShader& shader,
                           const glm::vec2& viewport)
{
    if (isHidden()) {
        return;
    }
    if (m_isGeometryUpdateNeeded) {
        updateGeometry(font);
    }
    glm::mat4 modelMatrix{1.0f};
    float scale = m_fontSize / font.getBitmapSize();

    auto transform = m_position.apply(viewport);

    translateMatrix(modelMatrix, {transform.x, transform.y, 0.0f});
    rotateMatrix(modelMatrix, {180.0f, 0.0f, 0.0f});
    scaleMatrix(modelMatrix, scale);

    shader.updateTransform(modelMatrix);

    m_textQuads.getDrawable().bindAndDraw();
}

void TextComponent::updateGeometry(const gl::Font& font)
{
    m_textQuads.destroy();
    Mesh mesh;
    m_isGeometryUpdateNeeded = false;

    sf::Vector2f pos{0, 0};
    char previous = 0;
    for (auto character : m_text) {
        pos.x += font.getKerning(previous, character);
        previous = character;

        // New line handler
        if (character == '\n') {
            pos.y += font.getLineHeight();
            pos.x = 0;
            previous = 0;
            continue;
        }

        // Create a single quad for the char
        auto& glyph = font.getGlyph(character);
        addCharacter(mesh, glyph, font.getTextureAtlasSize(), pos);
        pos.x += glyph.advance;
    }

    m_textQuads.create();
    m_textQuads.bind();
    m_textQuads.addVertexBuffer(2, mesh.vertices);
    m_textQuads.addVertexBuffer(2, mesh.textureCoords);
    m_textQuads.addIndexBuffer(mesh.indices);
}

} // namespace gui