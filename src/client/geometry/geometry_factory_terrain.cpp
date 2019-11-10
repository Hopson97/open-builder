#include "geometry_factory.h"

#include <cstdlib>

namespace client::GeometryFactory {
    Mesh createTerrain(int width, int depth)
    {
        Mesh mesh;

        std::vector<float> heightMap;

        for (int z = 0; z < depth; z++) {
            for (int x = 0; x < width; x++) {
                if (x == 0 || z == 0 || z == depth - 1 || x == width - 1) {
                    heightMap.push_back(1.0f);
                }
                else {
                    heightMap.push_back(float(rand() % 10) / 32.0f);
                }
            }
        }

        for (int z = 0; z < depth; z++) {
            for (int x = 0; x < width; x++) {
                auto getHeight = [&heightMap, width](int xp, int zp) {
                    return heightMap[zp * width + xp];
                };
                auto h = getHeight(x, z);
                mesh.vertices.push_back(x);
                mesh.vertices.push_back(h);
                mesh.vertices.push_back(z);

                mesh.textureCoords.push_back(x % 2);
                mesh.textureCoords.push_back(z % 2);

                if (x > 0 && x < width - 1 && z > 0 && z < depth - 1) {
                    float heightL = getHeight(x - 1, z);
                    float heightR = getHeight(x + 1, z);
                    float heightD = getHeight(x, z - 1);
                    float heightU = getHeight(x, z + 1);
                    glm::vec3 n{heightL - heightR, 2.0f, heightD - heightU};
                    auto normal = glm::normalize(n);

                    mesh.normals.push_back(normal.x);
                    mesh.normals.push_back(normal.y);
                    mesh.normals.push_back(normal.z);
                }
                else {
                    mesh.normals.push_back(0);
                    mesh.normals.push_back(1);
                    mesh.normals.push_back(0);
                }
            }
        }

        for (int z = 0; z < depth - 1; z++) {
            for (int x = 0; x < width - 1; x++) {
                int i = x + z * width;
                mesh.indices.push_back(i);
                mesh.indices.push_back(i + width);
                mesh.indices.push_back(i + width + 1);
                mesh.indices.push_back(i + width + 1);
                mesh.indices.push_back(i + 1);
                mesh.indices.push_back(i);
            }
        }
        return mesh;
    }
} // namespace client::GeometryFactory
