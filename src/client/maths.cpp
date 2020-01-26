#include "maths.h"

#include <common/world/world_constants.h>

// ===============================================
//
//      General vector and matricies helpers
//
// ===============================================
glm::mat4 createProjectionViewMatrix(const glm::vec3 &position,
                                     const glm::vec3 &rotation,
                                     const glm::mat4 &projection)
{
    glm::mat4 view{1.0f};
    glm::mat4 projectionView{1.0f};

    rotateMatrix(view, rotation);
    translateMatrix(view, -position);
    projectionView = projection * view;

    return projectionView;
}

void rotateMatrix(glm::mat4 &matrix, const glm::vec3 &degrees)
{
    matrix = glm::rotate(matrix, glm::radians(degrees.x), {1, 0, 0});
    matrix = glm::rotate(matrix, glm::radians(degrees.y), {0, 1, 0});
    matrix = glm::rotate(matrix, glm::radians(degrees.z), {0, 0, 1});
}

void translateMatrix(glm::mat4 &matrix, const glm::vec3 &offset)
{
    matrix = glm::translate(matrix, offset);
}

void scaleMatrix(glm::mat4 &matrix, const glm::vec3 &scalars)
{
    matrix = glm::scale(matrix, scalars);
}

void scaleMatrix(glm::mat4 &matrix, float scalar)
{
    scaleMatrix(matrix, {scalar, scalar, scalar});
}

glm::vec3 forwardsVector(const glm::vec3 &rotation)
{
    float yaw = glm::radians(rotation.y + 90);
    float pitch = glm::radians(rotation.x);
    float x = glm::cos(yaw) * glm::cos(pitch);
    float y = glm::sin(pitch);
    float z = glm::cos(pitch) * glm::sin(yaw);

    return {-x, -y, -z};
}

glm::vec3 backwardsVector(const glm::vec3 &rotation)
{
    return -forwardsVector(rotation);
}

glm::vec3 leftVector(const glm::vec3 &rotation)
{
    float yaw = glm::radians(rotation.y);
    float x = glm::cos(yaw);
    float y = 0;
    float z = glm::sin(yaw);

    return {-x, -y, -z};
}

glm::vec3 rightVector(const glm::vec3 &rotation)
{
    return -leftVector(rotation);
}

// ===============================================
//
//                  Ray casts
//
// ===============================================
Ray::Ray(const glm::vec3 &startPosition, const glm::vec3 &direction)
    : m_start(startPosition)
    , m_previous(startPosition)
    , m_end(startPosition)
    , m_direction(direction)

{
}

void Ray::step()
{
    m_previous = m_end;
    m_end += forwardsVector(m_direction) / 4.0f;
}

float Ray::getLength() const
{
    return glm::length(m_end - m_start);
}

const glm::vec3 &Ray::getEndpoint() const
{
    return m_end;
}

const glm::vec3 &Ray::getLastPoint() const
{
    return m_previous;
}

// ===============================================
//
//           Frustum/ Frustum Culling
//
// ===============================================
enum Planes {
    Near,
    Far,
    Left,
    Right,
    Top,
    Bottom,
};

float ViewFrustum::Plane::distanceToPoint(const glm::vec3 &point) const
{
    return glm::dot(point, normal) + distanceToOrigin;
}

void ViewFrustum::update(const glm::mat4 &mat) noexcept
{
    // Left
    m_planes[Planes::Left].normal.x = mat[0][3] + mat[0][0];
    m_planes[Planes::Left].normal.y = mat[1][3] + mat[1][0];
    m_planes[Planes::Left].normal.z = mat[2][3] + mat[2][0];
    m_planes[Planes::Left].distanceToOrigin = mat[3][3] + mat[3][0];

    // Right
    m_planes[Planes::Right].normal.x = mat[0][3] - mat[0][0];
    m_planes[Planes::Right].normal.y = mat[1][3] - mat[1][0];
    m_planes[Planes::Right].normal.z = mat[2][3] - mat[2][0];
    m_planes[Planes::Right].distanceToOrigin = mat[3][3] - mat[3][0];

    // Bottom
    m_planes[Planes::Bottom].normal.x = mat[0][3] + mat[0][1];
    m_planes[Planes::Bottom].normal.y = mat[1][3] + mat[1][1];
    m_planes[Planes::Bottom].normal.z = mat[2][3] + mat[2][1];
    m_planes[Planes::Bottom].distanceToOrigin = mat[3][3] + mat[3][1];

    // Top
    m_planes[Planes::Top].normal.x = mat[0][3] - mat[0][1];
    m_planes[Planes::Top].normal.y = mat[1][3] - mat[1][1];
    m_planes[Planes::Top].normal.z = mat[2][3] - mat[2][1];
    m_planes[Planes::Top].distanceToOrigin = mat[3][3] - mat[3][1];

    // Near
    m_planes[Planes::Near].normal.x = mat[0][3] + mat[0][2];
    m_planes[Planes::Near].normal.y = mat[1][3] + mat[1][2];
    m_planes[Planes::Near].normal.z = mat[2][3] + mat[2][2];
    m_planes[Planes::Near].distanceToOrigin = mat[3][3] + mat[3][2];

    // Far
    m_planes[Planes::Far].normal.x = mat[0][3] - mat[0][2];
    m_planes[Planes::Far].normal.y = mat[1][3] - mat[1][2];
    m_planes[Planes::Far].normal.z = mat[2][3] - mat[2][2];
    m_planes[Planes::Far].distanceToOrigin = mat[3][3] - mat[3][2];

    for (auto &plane : m_planes) {
        float length = glm::length(plane.normal);
        plane.normal /= length;
        plane.distanceToOrigin /= length;
    }
}

bool ViewFrustum::chunkIsInFrustum(ChunkPosition box) const noexcept
{
    box *= CHUNK_SIZE;

    auto getVP = [&](const glm::vec3 &normal) {
        auto res = box;

        if (normal.x > 0) {
            res.x += CHUNK_SIZE;
        }
        if (normal.y > 0) {
            res.y += CHUNK_SIZE;
        }
        if (normal.z > 0) {
            res.z += CHUNK_SIZE;
        }

        return glm::vec3{res.x, res.y, res.z};
    };

    auto getVN = [&](const glm::vec3 &normal) {
        auto res = box;

        if (normal.x < 0) {
            res.x += CHUNK_SIZE;
        }
        if (normal.y < 0) {
            res.y += CHUNK_SIZE;
        }
        if (normal.z < 0) {
            res.z += CHUNK_SIZE;
        }

        return glm::vec3{res.x, res.y, res.z};
    };

    bool result = true;
    for (auto &plane : m_planes) {
        if (plane.distanceToPoint(getVP(plane.normal)) < 0) {
            return false;
        }
        else if (plane.distanceToPoint(getVN(plane.normal)) < 0) {
            result = true;
        }
    }
    return result;
}