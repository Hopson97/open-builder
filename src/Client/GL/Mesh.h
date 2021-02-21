#pragma once

#include <glad/glad.h>

#include <glm/common.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace glpp {

    struct Mesh {
        std::vector<GLfloat> positions;
        std::vector<GLfloat> textureCoords;
        std::vector<GLfloat> normals;
        std::vector<GLuint> indices;
        int currentIndex = 0;
    };

    Mesh createScreenMesh();
    Mesh createCubeMesh(const glm::vec3& dimensions);
    Mesh createWireCubeMesh(const glm::vec3& dimensions, float wireThickness = 0.1);
    Mesh createTerrainMesh(int terrainZIndex, const glm::vec2& size, float tileSize);
} // namespace glpp