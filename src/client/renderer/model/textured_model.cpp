#include "textured_model.h"

#include "../../geometry/geometry_factory.h"
#include "../../util/model_loader.h"

namespace client {
    TexturedModel::TexturedModel()
    {
        create(GeometryFactory::createCube({0, 0, 0}, {1, 2, 1}), "empty.png");
    }

    TexturedModel::TexturedModel(const std::string &modelFileName,
                                 const std::string &textureFileName)
        : m_texture(textureFileName)
    {
        auto mesh = loadModel(modelFileName);
        m_model.create(mesh);
    }

    TexturedModel::TexturedModel(TexturedModel &&other)
        : m_model(std::move(other.m_model))
        , m_texture(std::move(other.m_texture))
    {
    }

    TexturedModel::TexturedModel(const Mesh &mesh,
                                 const std::string &textureFileName)
    {
        create(mesh, textureFileName);
    }

    TexturedModel &TexturedModel::operator=(TexturedModel &&other)
    {
        m_model = std::move(other.m_model);
        m_texture = std::move(other.m_texture);

        return *this;
    }

    void TexturedModel::create(const std::string &modelFileName,
                               const std::string &textureFileName)
    {
        auto mesh = loadModel(modelFileName);
        m_model.create(mesh);
        m_texture.create(textureFileName);
    }

    void TexturedModel::create(const Mesh &mesh,
                               const std::string &textureFileName)
    {
        m_model.create(mesh);
        m_texture.create(textureFileName);
    }

    void TexturedModel::bind() const
    {
        m_model.bind();
        m_texture.bind();
    }

    GLsizei TexturedModel::getIndicesCount() const
    {
        return m_model.getIndicesCount();
    }

} // namespace client
