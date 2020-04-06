#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "world/coordinate.h"
#include <array>
#include <cfloat>
#include <vector>

glm::mat4 createProjectionViewMatrix(const glm::vec3& position, const glm::vec3& rotation,
                                     const glm::mat4& projection);

/**
 * @brief Rotate a matrix
 *
 * @param matrix The matrix to rotate
 * @param degrees Rotation axis in degrees
 */
void rotateMatrix(glm::mat4& matrix, const glm::vec3& degrees);

/**
 * @brief Translates a matrix
 *
 * @param matrix The matrix to translate
 * @param offset The amount of translate by
 */
void translateMatrix(glm::mat4& matrix, const glm::vec3& offset);

/**
 * @brief Scales a matrix
 *
 * @param matrix The matrix to scale
 * @param scalars The amount to scale each axis by
 */
void scaleMatrix(glm::mat4& matrix, const glm::vec3& scalars);

/**
 * @brief Scales a matrix
 *
 * @param matrix The matrix to scale
 * @param scalar The amount to scale the whole matrix by
 */
void scaleMatrix(glm::mat4& matrix, float scalar);

/**
 * @brief Creates a forwards-facing vector
 *
 * @param rotation The rotation axis to base the result on
 * @return glm::vec3 A vector that is forwards from the rotation vector
 */
glm::vec3 forwardsVector(const glm::vec3& rotation);

/**
 * @brief Creates a backwards-facing vector
 *
 * @param rotation The rotation axis to base the result on
 * @return glm::vec3 A vector that is backwards from the rotation vector
 */
glm::vec3 backwardsVector(const glm::vec3& rotation);

/**
 * @brief Creates a left-facing vector
 *
 * @param rotation The rotation axis to base the result on
 * @return glm::vec3 A vector that is left from the rotation vector
 */
glm::vec3 leftVector(const glm::vec3& rotation);

/**
 * @brief Creates a right-facing vector
 *
 * @param rotation The rotation axis to base the result on
 * @return glm::vec3 A vector that is right from the rotation vector
 */
glm::vec3 rightVector(const glm::vec3& rotation);

/**
 * @brief Returns a list of intersected VoxelPositions given a starting point, direction,
 * and range
 *
 * @param startPoint The position at which to begin testing for intersections
 * @param direction Direction to travel away from the start point
 * @param range Maximum distance to travel from the start point
 */
std::vector<VoxelPosition> getIntersectedVoxels(const glm::vec3& startPoint,
                                                const glm::vec3& direction, float range);

/**
 * @brief Represents a stepable ray
 *
 */
class Ray {
  public:
    Ray(const glm::vec3& startPosition, const glm::vec3& direction);

    /**
     * @brief Step the raycast in its direction
     */
    void step();

    /**
     * @brief Gets the length of the raycast, from the start to endpoint
     * @return float The length of the raycast
     */
    float getLength() const;

    /**
     * @brief Gets the current endpoint of the ray
     * @return const glm::vec3& The endpoint of the ray
     */
    const glm::vec3& getEndpoint() const;

    /**
     * @brief Gets the previous endpoint of the ray
     * @return const glm::vec3& The previous endpoint of the ray
     */
    const glm::vec3& getLastPoint() const;

  private:
    glm::vec3 m_start;
    glm::vec3 m_previous;
    glm::vec3 m_end;
    glm::vec3 m_direction;
};

class ViewFrustum {
    struct Plane {
        float distanceToPoint(const glm::vec3& point) const;

        float distanceToOrigin;
        glm::vec3 normal;
    };

  public:
    void update(const glm::mat4& projViewMatrix) noexcept;

    bool chunkIsInFrustum(ChunkPosition position) const noexcept;

  private:
    std::array<Plane, 6> m_planes;
};
