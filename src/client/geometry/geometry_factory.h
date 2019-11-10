#pragma once

#include "../renderer/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace client::GeometryFactory {
    Mesh createQuad(float width, float depth);
    Mesh createCube(const glm::vec3 &min, const glm::vec3 &max);
    Mesh createCubeWithNormals(const glm::vec3 &min, const glm::vec3 &max);

    Mesh createTerrain(int width, int depth);

    void addBox(Mesh &mesh, const glm::vec3 &min, const glm::vec3 &max);

    std::vector<GLfloat> createInvertedCubeVerticies(const glm::vec3 &min,
                                                     const glm::vec3 &max);
    std::vector<GLfloat> createCubeVerticies(const glm::vec3 &min,
                                             const glm::vec3 &max);
    std::vector<GLuint> createCubeIndices(GLuint start = 0);
} // namespace client::GeometryFactory
